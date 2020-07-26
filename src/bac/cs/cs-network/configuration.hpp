
#pragma once

#include <string>

namespace bac::cs::net {

    class ServerConfig
    {
    public:
        std::string address;
        unsigned short port = 0;
    };

    enum class AutoReconnect
    {
        No,
        Yes,
    };

    class ClientConfig
    {
    public:
        ServerConfig server;
        AutoReconnect auto_reconnect = AutoReconnect::Yes;
    };

} // namespace bac::cs::net
