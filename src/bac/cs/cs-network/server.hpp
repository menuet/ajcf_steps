
#pragma once

#include "configuration.hpp"
#include "connection.hpp"
#include <functional>
#include <memory>

namespace asio {
    class io_context;
} // namespace asio

namespace bac::cs::net {

    class Server
    {
    public:
        using Config = ServerConfig;

        Server(Config config, asio::io_context& io_context);

        ~Server();

        bool start(ConnectionEventHandler connection_event_handler);

    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;
    };

} // namespace bac::cs::net
