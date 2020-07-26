
#include "client_app.hpp"
#include <bac/cs/cs-domain/actors.hpp>
#include <bac/cs/cs-network/client.hpp>
#include <bac/cs/cs-network/messages.hpp>
#include <bac/domain/logic.hpp>
#include <bac/util/logs.hpp>

namespace bac::cs::client {

    class Application::Impl : public std::enable_shared_from_this<Application::Impl>
    {
    public:
        Impl(Config config, asio::io_context& io_context)
            : m_config(std::move(config)), m_client(m_config.client, io_context)
        {
        }

        bool start()
        {
            return m_client.start([this, shared_this = shared_from_this()](net::ClientEvent client_event) {
                switch (client_event.type)
                {
                case net::ClientEventType::Status:
                    switch (client_event.status)
                    {
                    case net::ClientStatus::Disconnected:
                        BAC_LOG_COUT(INFO, APP, "Client event Disconnected");
                        break;
                    case net::ClientStatus::Connecting:
                        BAC_LOG_COUT(INFO, APP, "Client event Connecting");
                        break;
                    case net::ClientStatus::Connected:
                        BAC_LOG_COUT(INFO, APP, "Client event Connected");
                        begin();
                        break;
                    }
                    break;
                case net::ClientEventType::Message:
                    BAC_LOG_COUT(INFO, APP, "Client event Message '" << client_event.message << "'");
                    proto::handle_message(client_event.message,
                                          [this](const auto& payload) { payload_handler(payload); });
                    break;
                }
            });
        }

    private:
        void begin()
        {
            // TODO
            m_client.send_message("PING"); // TODO: remove
        }

        template <typename PayloadT>
        void payload_handler(const PayloadT&)
        {
        }

        void payload_handler([[maybe_unused]] const proto::Payload_RequestAttempt& payload)
        {
            // TODO
        }

        void payload_handler([[maybe_unused]] const proto::Payload_RequestFeedback& payload)
        {
            // TODO
        }

        Config m_config;
        net::Client m_client;
        actors::GameActor m_game_actor;
        std::unique_ptr<actors::CodebreakerActor> m_codebreaker_actor;
        std::unique_ptr<actors::CodemakerActor> m_codemaker_actor;
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

} // namespace bac::cs::client
