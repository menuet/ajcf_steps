
#pragma once

#include <condition_variable>
#include <type_traits>
#include <cassert>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

namespace bac {

    // FIFO queue for one producer thread, one consumer thread
    template <typename T>
    class ConcurrentQueue
    {
        std::deque<T> m_queue{};
        std::mutex m_mutex{};
        std::condition_variable m_cv{};

    public:
        // Push some data to the queue
        template <typename TT>
        void push(TT&& data)
        {
            {
                std::lock_guard lock{m_mutex};
                m_queue.push_back(std::forward<TT>(data));
            }
            m_cv.notify_all();
        }

        // Wait until some data is in the queue, then pop it
        T pop()
        {
            std::unique_lock lock{m_mutex};
            m_cv.wait(lock, [this] { return !m_queue.empty(); });
            assert(!m_queue.empty());
            T data = std::move(m_queue.front());
            m_queue.pop_front();
            return data;
        }
    };

    // Object containing one thread that executes
    // all tasks that are pushed to its queue
    class Actor
    {
    private:
        using Task = std::function<void()>;

        ConcurrentQueue<Task> m_tasks{};
        std::thread m_thread{};

    public:
        ~Actor()
        {
            m_tasks.push(Task{});
            m_thread.join();
        }

        Actor()
            : m_thread{[this] {
                  for (;;)
                  {
                      auto task = m_tasks.pop();
                      if (!task)
                          break;
                      task();
                  }
              }}
        {
        }

        // Push a task to be executed by the actor's thread
        template <typename TaskT>
        void push_task(TaskT&& task)
        {
            m_tasks.push(std::forward<TaskT>(task));
        }
    };

    template <typename Callable>
    using return_type_of = typename decltype(std::function{std::declval<Callable>()})::result_type;

    // Continuator object returned by the stand-alone push_task
    template <typename PreviousComputationResultT>
    struct Continuator
    {
        Continuator(std::promise<PreviousComputationResultT>& promise) : m_future{promise.get_future()}
        {
        }

        // Attach a continuation to this continuator,
        // to be executed on the specified actor
        template <typename ComputationT>
        [[nodiscard]] auto then(Actor& actor, ComputationT computation)
        {
            using ComputationResult = return_type_of<ComputationT>;
            auto prom = std::make_shared<std::promise<ComputationResult>>();
            Continuator<ComputationResult> cont{*prom};
            actor.push_task([this, computation = std::move(computation), prom = std::move(prom)]() {
                if constexpr (!std::is_void_v<PreviousComputationResultT>)
                {
                    auto input = m_future.get();
                    if constexpr (!std::is_void_v<decltype(computation(input))>)
                    {
                        auto result = computation(input);
                        prom->set_value(result);
                    }
                    else
                    {
                        computation(input);
                        prom->set_value();
                    }
                }
                else
                {
                    m_future.wait();
                    if constexpr (!std::is_void_v<decltype(computation())>)
                    {
                        auto result = computation();
                        prom->set_value(result);
                    }
                    else
                    {
                        computation();
                        prom->set_value();
                    }
                }
            });
            return cont;
        }

        // Wait for the computation to finish instead of attaching a continuation
        void wait()
        {
            m_future.wait();
        }

        // Wait for the computation to finish instead of attaching a continuation
        // and get the result
        PreviousComputationResultT get()
        {
            return m_future.get();
        }

    private:
        std::future<PreviousComputationResultT> m_future;
    };

    // Push a computation to an actor's thread
    // and return a continuator object
    // to which one can attach a computation task, possibly on another actor
    template <typename ComputationT>
    [[nodiscard]] auto push_task(Actor& actor, ComputationT computation)
    {
        using ComputationResult = return_type_of<ComputationT>;
        auto prom = std::make_shared<std::promise<ComputationResult>>();
        Continuator<ComputationResult> cont{*prom};
        actor.push_task([computation = std::move(computation), prom = std::move(prom)]() {
            if constexpr (!std::is_void_v<decltype(computation())>)
            {
                auto result = computation();
                prom->set_value(result);
            }
            else
            {
                computation();
                prom->set_value();
            }
        });
        return cont;
    }

} // namespace bac
