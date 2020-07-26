
#pragma once

#include <bac/cs/cs-domain/protocol.hpp>
#include <bac/cs/cs-network/configuration.hpp>
#include <bac/domain/options.hpp>
#include <memory>

namespace asio {
    class io_context;
} // namespace asio

namespace bac::cs::client {

    class Application
    {
    public:
        class Config
        {
        public:
            net::ClientConfig client;
            proto::PlayerRole player_role;
            Options options;
        };

        Application(Config config, asio::io_context& io_context);

        ~Application();

        bool start();

    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;
    };

} // namespace bac::cs::client
