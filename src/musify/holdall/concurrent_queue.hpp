
#pragma once

#include <condition_variable>
#include <atomic>
#include <cassert>
#include <deque>
#include <mutex>
#include <optional>

namespace musify::actor {

    template <typename T>
    class ConcurrentQueue
    {
    public:
        template <typename AnyT>
        void push(AnyT&& data)
        {
            {
                std::lock_guard lock{m_mutex};
                m_queue.push_back(std::forward<AnyT>(data));
            }
            m_cv.notify_all();
        }

        void request_stop()
        {
            m_stop = true;
            m_cv.notify_all();
        }

        std::optional<T> wait_pop()
        {
            std::unique_lock lock{m_mutex};
            m_cv.wait(lock, [this] { return !m_queue.empty() || m_stop; });
            assert(!m_queue.empty() || m_stop);
            if (m_stop)
                return std::nullopt;
            T data = std::move(m_queue.front());
            m_queue.pop_front();
            return std::optional<T>{std::move(data)};
        }

    private:
        std::deque<T> m_queue{};
        std::mutex m_mutex{};
        std::condition_variable m_cv{};
        std::atomic<bool> m_stop{};
    };

} // namespace musify::actor
