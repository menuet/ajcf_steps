
#pragma once

#include "concurrent_queue.hpp"
#include <thread>

namespace musify::actor {

    class Actor
    {
    public:
        using Task = std::function<void()>;

        ~Actor()
        {
            m_pending_tasks.request_stop();
            m_thread.join();
        }

        Actor()
            : m_thread{[this] {
                  for (;;)
                  {
                      auto task_opt = m_pending_tasks.wait_pop();
                      if (!task_opt)
                          return;
                      auto& task = task_opt.value();
                      task();
                  }
              }}
        {
        }

        template <typename TaskT>
        void push_task(TaskT&& task)
        {
            m_pending_tasks.push(std::forward<TaskT>(task));
        }

    private:
        ConcurrentQueue<Task> m_pending_tasks{};
        std::thread m_thread{};
    };

} // namespace musify::actor
