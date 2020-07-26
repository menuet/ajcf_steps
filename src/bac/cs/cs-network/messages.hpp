
#pragma once

#include <string_view>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/system_error.hpp>
#include <asio/write.hpp>
#include <bac/util/logs.hpp>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <functional>
#include <vector>

namespace bac::cs::net {

    class Message
    {
    public:
        enum
        {
            MESSAGE_MAX_LENGTH = 9999999,
            MESSAGE_MAX_LENGTH_DIGITS_COUNT = 7,
            MESSAGE_HEADER_SIZE = MESSAGE_MAX_LENGTH_DIGITS_COUNT + 1,
        };

        Message()
        {
        }

        Message(std::string_view message)
        {
            const auto message_length = static_cast<std::int32_t>(
                message.length() > MESSAGE_MAX_LENGTH ? MESSAGE_MAX_LENGTH : message.length());
            m_message.resize(MESSAGE_HEADER_SIZE + 1);
            std::sprintf(m_message.data(), "%0*d:", MESSAGE_MAX_LENGTH_DIGITS_COUNT, message_length);
            m_message.resize(MESSAGE_HEADER_SIZE);
            m_message.insert(m_message.end(), message.begin(), message.begin() + message_length);
            BAC_LOG_COUT(INFO, WRITE, "Creating message header+body: '" << data() << "'");
        }

        asio::const_buffers_1 const_buffer() const
        {
            return asio::buffer(m_message.data(), m_message.size());
        }

        std::string_view data() const
        {
            return std::string_view(m_message.data(), m_message.size());
        }

        void adapt_buffer_for_header()
        {
            m_message.clear();
            m_message.resize(MESSAGE_HEADER_SIZE);
        }

        void adapt_buffer_for_body(std::error_code& ec)
        {
            if (m_message.size() != MESSAGE_HEADER_SIZE)
            {
                ec = std::make_error_code(std::errc::bad_message);
                return;
            }

            if (m_message[m_message.size() - 1] != ':')
            {
                ec = std::make_error_code(std::errc::bad_message);
                return;
            }

            m_message[m_message.size() - 1] = 0;
            const auto message_length = std::atoi(m_message.data());
            if (message_length <= 0 || message_length > MESSAGE_MAX_LENGTH)
            {
                ec = std::make_error_code(std::errc::bad_message);
                return;
            }

            m_message.clear();
            m_message.resize(message_length);
        }

        asio::mutable_buffers_1 mutable_buffer()
        {
            return asio::buffer(m_message.data(), m_message.size());
        }

    private:
        std::vector<char> m_message;
    };

    class MessageWriter
    {
    public:
        MessageWriter(asio::ip::tcp::socket& socket) : m_socket(socket)
        {
        }

        void write(std::string_view message)
        {
            asio::post(m_socket.get_executor(), [this, message = Message(message)]() mutable {
                const auto write_in_progress = !m_pending_messages.empty();
                m_pending_messages.push_back(std::move(message));
                if (!write_in_progress)
                    do_write();
            });
        }

    private:
        void do_write()
        {
            const auto& message = m_pending_messages.front();
            BAC_LOG_COUT(INFO, WRITE, "Writing message: '" << message.data() << "'");
            asio::async_write(m_socket, message.const_buffer(),
                              [this](std::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
                                  if (ec)
                                  {
                                      BAC_LOG_COUT(INFO, WRITE, "Error when writing message");
                                      m_socket.close();
                                      return;
                                  }

                                  BAC_LOG_COUT(INFO, WRITE, "Success when writing message");

                                  m_pending_messages.pop_front();
                                  if (!m_pending_messages.empty())
                                      do_write();
                              });
        }

        asio::ip::tcp::socket& m_socket;
        std::deque<Message> m_pending_messages;
    };

    class MessageReader
    {
    public:
        typedef std::function<void(const std::error_code&, std::string_view)> MessageHandler;

        MessageReader(asio::ip::tcp::socket& socket) : m_socket(socket), m_message()
        {
        }

        void read_messages(MessageHandler message_handler)
        {
            m_message_handler = std::move(message_handler);
            do_read_header();
        }

    private:
        void do_read_header()
        {
            m_message.adapt_buffer_for_header();
            BAC_LOG_COUT(INFO, WRITE, "Reading message header (size adapted to: " << m_message.data().size() << ")");
            asio::async_read(
                m_socket, m_message.mutable_buffer(),
                [this](std::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
                    if (!ec)
                    {
                        BAC_LOG_COUT(INFO, READ, "Success when reading message header: '" << m_message.data() << "'");
                        m_message.adapt_buffer_for_body(ec);
                    }

                    if (ec)
                    {
                        BAC_LOG_COUT(INFO, READ, "Error when reading message header or adapting message size for body");
                        m_socket.close();
                        m_message_handler(ec, std::string());
                        return;
                    }

                    BAC_LOG_COUT(INFO, READ,
                                 "Reading message body (size adapted to: " << m_message.data().size() << ")");
                    do_read_body();
                });
        }

        void do_read_body()
        {
            asio::async_read(m_socket, m_message.mutable_buffer(),
                             [this](std::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
                                 if (ec)
                                 {
                                     BAC_LOG_COUT(INFO, READ, "Error when reading message body");
                                     m_socket.close();
                                     m_message_handler(ec, std::string());
                                     return;
                                 }

                                 BAC_LOG_COUT(INFO, READ,
                                              "Success when reading message body: '" << m_message.data() << "'");
                                 m_message_handler(ec, m_message.data());

                                 do_read_header();
                             });
        }

        asio::ip::tcp::socket& m_socket;
        Message m_message;
        MessageHandler m_message_handler;
    };

} // namespace bac::cs::net
