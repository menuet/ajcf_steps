
#pragma once

#include <asio/high_resolution_timer.hpp>
#include <chrono>
#include <functional>

namespace asio {
    class io_context;
} // namespace asio

namespace bac::cs::net {

    class Scheduler
    {
    public:
        typedef std::function<void()> Handler;

        Scheduler(asio::io_context& io_context, std::chrono::milliseconds interval)
            : m_timer(io_context), m_interval(interval)
        {
        }

        template <typename HandlerT>
        void start(HandlerT&& handler)
        {
            m_handler = std::forward<HandlerT>(handler);
            do_wait();
        }

        void stop()
        {
            m_timer.cancel();
        }

    private:
        void do_wait()
        {
            m_timer.expires_after(m_interval);
            m_timer.async_wait([this](const auto&) {
                m_handler();
                do_wait();
            });
        }

        asio::high_resolution_timer m_timer;
        std::chrono::milliseconds m_interval;
        Handler m_handler;
    };

} // namespace bac::cs::net
