
#pragma once

#include <string_view>
#include <asio/ip/tcp.hpp>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <memory>
#include <string>

namespace bac::cs::net {

    class ConnectionId
    {
    public:
        ConnectionId(std::int64_t id) : id(id)
        {
        }

        bool operator==(ConnectionId connection_id) const
        {
            return id == connection_id.id;
        }

        bool operator!=(ConnectionId connection_id) const
        {
            return id != connection_id.id;
        }

        bool operator<(ConnectionId connection_id) const
        {
            return id < connection_id.id;
        }

        friend inline std::ostream& operator<<(std::ostream& os, ConnectionId connection_id)
        {
            os << connection_id.id;
            return os;
        }

    private:
        std::int64_t id;
    };

    enum class ConnectionEventType
    {
        Create,
        Destroy,
        Message,
    };

    class ConnectionEvent;

    typedef std::function<void(ConnectionEvent)> ConnectionEventHandler;

    class Connection
    {
    public:
        using Task = std::function<void()>;

        Connection(ConnectionId connection_id, asio::ip::tcp::socket socket,
                   ConnectionEventHandler connection_event_handler);

        ~Connection();

        ConnectionId id() const;

        void send_message(std::string_view message) const;

        void disconnect() const;

        void post(Task task);

    private:
        class Impl;

        Connection(std::shared_ptr<Impl> impl);

        std::shared_ptr<Impl> m_impl;
    };

    class ConnectionEvent
    {
    public:
        ConnectionEvent(Connection connection, ConnectionEventType type, std::string message)
            : connection(std::move(connection)), type(type), message(std::move(message))
        {
        }

        Connection connection;
        ConnectionEventType type;
        std::string message;
    };

} // namespace bac::cs::net
