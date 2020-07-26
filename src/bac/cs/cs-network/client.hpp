
#pragma once

#include "configuration.hpp"
#include "scheduler.hpp"
#include <functional>
#include <memory>
#include <string>

namespace asio {
    class io_context;
} // namespace asio

namespace bac::cs::net {

    enum class ClientStatus
    {
        Disconnected,
        Connecting,
        Connected,
    };

    enum class ClientEventType
    {
        Status,
        Message,
    };

    class ClientEvent
    {
    public:
        ClientEvent(ClientStatus status) : type(ClientEventType::Status), status(status), message()
        {
        }

        ClientEvent(std::string message)
            : type(ClientEventType::Message), status(ClientStatus::Disconnected), message(std::move(message))
        {
        }

        ClientEventType type;
        ClientStatus status;
        std::string message;
    };

    typedef std::function<void(ClientEvent)> ClientEventHandler;

    class Client
    {
    public:
        using Config = ClientConfig;
        using Task = std::function<void()>;

        Client(Config config, asio::io_context& io_context);

        ~Client();

        bool start(ClientEventHandler client_event_handler);

        void send_message(std::string_view message);

        void disconnect(AutoReconnect auto_reconnect);

        void post(Task task);

    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;
    };

} // namespace bac::cs::net
