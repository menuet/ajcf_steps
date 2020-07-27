
#include "server_app.hpp"
#include <bac/cs/cs-domain/actors.hpp>
#include <bac/cs/cs-domain/protocol.hpp>
#include <bac/cs/cs-network/messages.hpp>
#include <bac/cs/cs-network/server.hpp>

namespace bac::cs::server {

    class Application::Impl : public std::enable_shared_from_this<Application::Impl>
    {
    public:
        Impl(Config config, asio::io_context& io_context)
            : m_config(std::move(config)), m_server(m_config.server, io_context)
        {
        }

        bool start()
        {
            {
                const auto result =
                    m_server.start([this, shared_this = shared_from_this()](net::ConnectionEvent connection_event) {
                        handle_connection_event(std::move(connection_event));
                    });
                if (!result)
                    return false;
            }

            return true;
        }

    private:
        void handle_connection_event(net::ConnectionEvent connection_event)
        {
            switch (connection_event.type)
            {
            case net::ConnectionEventType::Create:
                handle_connection_event_create(std::move(connection_event.connection));
                break;
            case net::ConnectionEventType::Destroy:
                handle_connection_event_destroy(connection_event.connection.id());
                break;
            case net::ConnectionEventType::Message:
                handle_connection_event_message(connection_event.connection.id(), connection_event.message);
                break;
            }
        }

        void handle_connection_event_create(net::Connection connection)
        {
            BAC_LOG_COUT(INFO, APP, "New connection " << connection.id());
            m_connections.push_back({std::move(connection), nullptr});
        }

        void handle_connection_event_destroy(net::ConnectionId connection_id)
        {
            BAC_LOG_COUT(INFO, APP, "Cut connection " << connection_id);
            const auto iter = std::find_if(m_connections.begin(), m_connections.end(),
                                           [&](const auto& c_and_a) { return c_and_a.first.id() == connection_id; });
            if (iter != m_connections.end())
                m_connections.erase(iter);
        }

        void handle_connection_event_message(net::ConnectionId connection_id, const std::string& message)
        {
            BAC_LOG_COUT(INFO, APP, "Message from connection " << connection_id << ": '" << message << "'");
            const auto iter = std::find_if(m_connections.begin(), m_connections.end(),
                                           [&](const auto& c_and_a) { return c_and_a.first.id() == connection_id; });
            if (iter != m_connections.end())
            {
                const auto& connection = iter->first;
                auto& game_actor = iter->second;
                proto::handle_message(message, [this, &connection, &game_actor](const auto& payload) {
                    payload_handler(connection, game_actor, payload);
                });
            }
        }

        using ConnectionAndGame = std::pair<net::Connection, std::shared_ptr<actors::GameActor>>;

        template <typename PayloadT>
        void payload_handler(const net::Connection&, std::shared_ptr<actors::GameActor>&, const PayloadT&)
        {
        }

        void payload_handler(const net::Connection& connection, std::shared_ptr<actors::GameActor>& game_actor,
                             const proto::Payload_RequestBegin& payload)
        {
            switch (payload.client_role)
            {
            case proto::PlayerRole::Codebreaker:
                game_actor = std::make_shared<actors::GameActor>();
                game_actor->begin(m_codemaker_actor, std::cout, payload.options);
                connection.send_message(proto::message_to_string(proto::Payload_RequestAttempt{}));
                break;
            case proto::PlayerRole::Codemaker:
                game_actor = std::make_shared<actors::GameActor>();
                game_actor->begin(m_codebreaker_actor, std::cout, payload.options);
                game_actor->request_attempt(
                    std::cout, std::cin, std::nullopt, std::nullopt, [connection](GameStatus, Code attempt) {
                        connection.send_message(proto::message_to_string(proto::Payload_RequestFeedback{attempt}));
                    });
                break;
            }
        }

        void payload_handler(const net::Connection& connection, std::shared_ptr<actors::GameActor>& game_actor,
                             const proto::Payload_RequestAttempt& payload)
        {
            game_actor->request_attempt(std::cout, std::cin, payload.feedback_opt, payload.secret_code_opt,
                                        [connection](GameStatus status, Code attempt) {
                                            if (status == GameStatus::Pending)
                                                connection.send_message(
                                                    proto::message_to_string(proto::Payload_RequestFeedback{attempt}));
                                        });
        }

        void payload_handler(const net::Connection& connection, std::shared_ptr<actors::GameActor>& game_actor,
                             const proto::Payload_RequestFeedback& payload)
        {
            game_actor->request_feedback(
                std::cout, payload.attempt,
                [connection, game_actor](Feedback feedback, std::optional<Code> secret_code_opt) {
                    connection.send_message(
                        proto::message_to_string(proto::Payload_RequestAttempt{feedback, secret_code_opt}));
                });
        }

        Config m_config;
        net::Server m_server;
        std::vector<ConnectionAndGame> m_connections;
        actors::CodemakerActor m_codemaker_actor;
        actors::CodebreakerActor m_codebreaker_actor;
    };

    Application::Application(Config config, asio::io_context& io_context)
        : m_impl(std::make_shared<Impl>(std::move(config), io_context))
    {
    }

    Application::~Application() = default;

    bool Application::start()
    {
        return m_impl->start();
    }

} // namespace bac::cs::server
