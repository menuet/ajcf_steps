
#include "client.hpp"
#include "messages.hpp"

namespace bac::cs::net {

    class Client::Impl : public std::enable_shared_from_this<Client::Impl>
    {
    public:
        Impl(Config config, asio::io_context& io_context)
            : m_config(std::move(config)), m_resolver(io_context), m_socket(io_context), m_message_reader(m_socket),
              m_message_writer(m_socket), m_status(ClientStatus::Disconnected),
              m_scheduler(io_context, std::chrono::seconds(5))
        {
        }

        bool start(ClientEventHandler client_event_handler)
        {
            m_client_event_handler = std::move(client_event_handler);

            asio::post(m_resolver.get_executor(), [this, shared_this = shared_from_this()]() { try_resolve(); });

            m_scheduler.start([this] {
                asio::post(m_resolver.get_executor(), [this, shared_this = shared_from_this()]() {
                    if (m_config.auto_reconnect == AutoReconnect::Yes && m_status == ClientStatus::Disconnected)
                        try_resolve();
                });
            });
            return true;
        }

        void send_message(std::string_view message)
        {
            m_message_writer.write(message);
        }

        void disconnect(AutoReconnect auto_reconnect)
        {
            asio::post(m_resolver.get_executor(), [this, shared_this = shared_from_this(), auto_reconnect]() {
                m_config.auto_reconnect = auto_reconnect;
                m_socket.close();
            });
        }

        void post(Task task)
        {
            asio::post(m_socket.get_executor(), std::move(task));
        }

    private:
        void try_resolve()
        {
            m_status = ClientStatus::Connecting;
            m_client_event_handler(ClientEvent(m_status));

            BAC_LOG_COUT(INFO, CLT, "Trying to resolve " << m_config.server.address << ":" << m_config.server.port);
            m_resolver.async_resolve(
                m_config.server.address, std::to_string(m_config.server.port),
                [this, shared_this = shared_from_this()](const std::error_code& err,
                                                         asio::ip::tcp::resolver::iterator endpoint_iterator) {
                    handle_resolve(err, endpoint_iterator);
                });
        }

        void handle_resolve(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator)
        {
            if (err)
            {
                BAC_LOG_COUT(INFO, CLT, "Error when trying to resolve");
                return;
            }

            try_connect(endpoint_iterator);
        }

        void try_connect(asio::ip::tcp::resolver::iterator endpoint_iterator)
        {
            const auto endpoint = *endpoint_iterator;
            BAC_LOG_COUT(INFO, CLT, "Trying to connect to " << endpoint.host_name() << ":" << endpoint.service_name());

            m_socket.async_connect(
                endpoint, [this, shared_this = shared_from_this(), endpoint_iterator](const std::error_code& err) {
                    handle_connect(err, endpoint_iterator);
                });
        }

        void handle_connect(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator)
        {
            if (err)
            {
                BAC_LOG_COUT(INFO, CLT, "Error when trying to connect");

                ++endpoint_iterator;
                if (endpoint_iterator == asio::ip::tcp::resolver::iterator())
                {
                    m_status = ClientStatus::Disconnected;
                    m_client_event_handler(ClientEvent(m_status));
                    return;
                }

                m_socket.close();

                try_connect(endpoint_iterator);
                return;
            }

            BAC_LOG_COUT(INFO, CLT, "Connected");

            m_status = ClientStatus::Connected;
            m_client_event_handler(ClientEvent(m_status));

            read_messages();
        }

        void read_messages()
        {
            m_message_reader.read_messages([this](const std::error_code& ec, std::string_view message) {
                if (ec)
                {
                    BAC_LOG_COUT(INFO, CLT, "Error while reading next message");
                    m_status = ClientStatus::Disconnected;
                    m_client_event_handler(ClientEvent(m_status));
                    return;
                }

                BAC_LOG_COUT(INFO, CLT, "Received message '" << message << "'");
                m_client_event_handler(ClientEvent(std::string(message)));
            });
        }

        Config m_config;
        asio::ip::tcp::resolver m_resolver;
        asio::ip::tcp::socket m_socket;
        MessageReader m_message_reader;
        MessageWriter m_message_writer;
        ClientEventHandler m_client_event_handler;
        ClientStatus m_status;
        Scheduler m_scheduler;
    };

    Client::Client(Config config, asio::io_context& io_context)
        : m_impl(std::make_shared<Impl>(std::move(config), io_context))
    {
    }

    Client::~Client() = default;

    bool Client::start(ClientEventHandler client_event_handler)
    {
        return m_impl->start(std::move(client_event_handler));
    }

    void Client::send_message(std::string_view message)
    {
        m_impl->send_message(message);
    }

    void Client::disconnect(AutoReconnect auto_reconnect)
    {
        m_impl->disconnect(auto_reconnect);
    }

    void Client::post(Task task)
    {
        m_impl->post(std::move(task));
    }

} // namespace bac::cs::net
