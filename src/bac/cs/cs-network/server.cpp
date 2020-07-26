
#include "server.hpp"
#include "connection.hpp"
#include "messages.hpp"
#include <vector>

namespace bac::cs::net {

    class Server::Impl : public std::enable_shared_from_this<Server::Impl>
    {
    public:
        Impl(Config config, asio::io_context& io_context)
            : m_config(std::move(config)), m_acceptor(io_context), m_next_connection_id(0)
        {
        }

        bool start(ConnectionEventHandler connection_event_handler)
        {
            m_connection_event_handler = std::move(connection_event_handler);

            std::error_code ec;

            BAC_LOG_COUT(INFO, SERV, "Opening protocol ip v4");
            m_acceptor.open(asio::ip::tcp::v4(), ec);
            if (ec)
            {
                BAC_LOG_COUT(ERROR, SERV, "Error when opening protocol : " << ec);
                return false;
            }

            BAC_LOG_COUT(INFO, SERV, "Binding " << m_config.address << ":" << m_config.port);
            const auto endpoint =
                asio::ip::tcp::endpoint(asio::ip::address::from_string(m_config.address), m_config.port);
            m_acceptor.bind(endpoint, ec);
            if (ec)
            {
                BAC_LOG_COUT(ERROR, SERV, "Error when binding : " << ec);
                return false;
            }

            BAC_LOG_COUT(INFO, SERV, "Listening " << m_config.address << ":" << m_config.port);
            m_acceptor.listen(asio::ip::tcp::acceptor::max_listen_connections, ec);
            if (ec)
            {
                BAC_LOG_COUT(ERROR, SERV, "Error when listening : " << ec);
                return false;
            }

            do_accept();
            return true;
        }

    private:
        void do_accept()
        {
            BAC_LOG_COUT(INFO, SERV, "Waiting for connections");

            m_acceptor.async_accept(
                [this, shared_this = shared_from_this()](const std::error_code& ec, asio::ip::tcp::socket socket) {
                    return handle_accept(ec, std::move(socket));
                });
        }

        void handle_accept(const std::error_code& ec, asio::ip::tcp::socket socket)
        {
            if (ec)
            {
                BAC_LOG_COUT(INFO, SERV, "Error when accepting connection");
            }
            else
            {
                BAC_LOG_COUT(INFO, SERV, "Accepted new connection");
                const auto connection_id = ++m_next_connection_id;
                Connection connection(connection_id, std::move(socket), m_connection_event_handler);
                m_connection_event_handler(ConnectionEvent(connection, ConnectionEventType::Create, std::string()));
            }

            do_accept();
        }

        Config m_config;
        asio::ip::tcp::acceptor m_acceptor;
        ConnectionEventHandler m_connection_event_handler;
        std::int64_t m_next_connection_id;
    };

    Server::Server(Config config, asio::io_context& io_context)
        : m_impl(std::make_shared<Impl>(std::move(config), io_context))
    {
    }

    Server::~Server() = default;

    bool Server::start(ConnectionEventHandler connection_event_handler)
    {
        return m_impl->start(std::move(connection_event_handler));
    }

} // namespace bac::cs::net
