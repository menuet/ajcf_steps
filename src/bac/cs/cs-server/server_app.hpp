
#pragma once

#include <bac/cs/cs-network/configuration.hpp>
#include <memory>

namespace asio {
    class io_context;
} // namespace asio

namespace bac::cs::server {

    class Application
    {
    public:
        class Config
        {
        public:
            net::ServerConfig server;
        };

        Application(Config config, asio::io_context& io_context);

        ~Application();

        bool start();

    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;
    };

} // namespace bac::cs::server
