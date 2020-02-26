
// https://en.cppreference.com/w/cpp/header/thread
// https://en.cppreference.com/w/cpp/header/mutex

#include <condition_variable>
#include <catch2/catch.hpp>
#include <array>
#include <cassert>
#include <chrono>
#include <deque>
#include <future>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>

using namespace std::literals;

namespace {

    TEST_CASE("started-then-joined thread", "[threads]")
    {
        std::stringstream thread_log{};

        // Create a c++ thread object
        // - which is associated with a newly created underlying system thread
        // - which executes a provided function (function address, function object, lambda, ...)
        std::thread cpp_thread{[&] {
            // ... Simulate a long computation ...
            // One can access the currently running thread via std::this_thread
            thread_log << "I am the thread " << std::this_thread::get_id() << " ";
            for (int i = 0; i != 3; ++i)
            {
                thread_log << ".";
                std::this_thread::sleep_for(100ms);
            }
            thread_log << " I am dying";
        }};

        // A thread object associated with a real system thread has a non-zero id
        REQUIRE(cpp_thread.get_id() != std::thread::id{});

        // A thread object associated with a real system thread has a non-null native handle
        REQUIRE(cpp_thread.native_handle());

        // A thread object associated with a real system thread is joinable
        REQUIRE(cpp_thread.joinable());

        // In most scenarios, at some point, one should wait for any created underlying system thread to finish
        // Block the currently running thread until the system thread associated with cpp_thread has terminated
        cpp_thread.join();

        // Now the underlying system thread is finished
        REQUIRE(!cpp_thread.joinable());
        REQUIRE(cpp_thread.get_id() == std::thread::id{});
        REQUIRE(!cpp_thread.native_handle());

        REQUIRE(std::regex_match(thread_log.str(), std::regex{R"(I am the thread \d+ ... I am dying)"}));
    }

    TEST_CASE("never-started thread", "[threads]")
    {
        // Create a c++ thread object which is not associated with any underlying system thread
        std::thread cpp_thread{};

        // The thread object is not associated with any real system thread
        REQUIRE(!cpp_thread.joinable());
        REQUIRE(cpp_thread.get_id() == std::thread::id{});
        REQUIRE(!cpp_thread.native_handle());
    }

    TEST_CASE("started-then-moved thread", "[threads]")
    {
        // Initially, cpp_thread1 is associated to an underlying system thread
        std::thread cpp_thread1{[&] {
            // ... Simulate a long computation ...
            std::this_thread::sleep_for(200ms);
        }};

        REQUIRE(cpp_thread1.joinable());

        // cpp_thread2 is not associated with anything
        std::thread cpp_thread2;

        REQUIRE(!cpp_thread2.joinable());

        const auto previous_cpp_thread1_id = cpp_thread1.get_id();

        // Now, cpp_thread2 is associated to the underlying system thread
        // cpp_thread1 is not associated anymore
        cpp_thread2 = std::move(cpp_thread1);

        REQUIRE(cpp_thread2.joinable());
        REQUIRE(cpp_thread2.get_id() == previous_cpp_thread1_id);
        REQUIRE(!cpp_thread1.joinable());

        cpp_thread2.join();
    }

    TEST_CASE("started-then-detached thread (DANGEROUS)", "[threads][!hide]")
    {
        int dummy_shared_data{};

        // Create a c++ thread object
        // - which is associated with a newly created underlying system thread
        // - which executes a provided function (function address, function object, lambda, ...)
        std::thread cpp_thread{[&] {
            // ... Simulate a long computation ...
            std::this_thread::sleep_for(200ms);
            // Undefined behavior !!!
            // The variable is not on the main thread's stack anymore when this thread accesses it
            dummy_shared_data = 42;
        }};

        // A thread object associated with a real system thread is joinable
        REQUIRE(cpp_thread.joinable());

        // One can detach the thread object from its underlying system thread
        // But this is dangerous
        cpp_thread.detach();

        // The thread object is not associated anymore with a real system thread
        // We cannot and must not join it
        REQUIRE(!cpp_thread.joinable());
        REQUIRE(cpp_thread.get_id() == std::thread::id{});
        REQUIRE(!cpp_thread.native_handle());
    }

    TEST_CASE("started-then-neither-joined-or-detached thread (FORBIDDEN)", "[threads][!hide]")
    {
        std::thread cpp_thread{[&] {
            // ... Simulate a long computation ...
            std::this_thread::sleep_for(200ms);
        }};

        // cpp_thread's destructor calls std::terminate !!!
        // because it is not detached from its underlying system thread
    }

    TEST_CASE("get a future from a promise which value is set on a separate thread", "[future]")
    {
        // Create the promise that will:
        // - contain the asynchronous result
        // - notify the future when the result is ready
        std::promise<int> prom;

        // Get the associated future
        std::future<int> fut = prom.get_future();

        // Launch the thread that performs the long computation asynchronously
        std::thread([&prom] {
            // ... Long computation ...
            std::this_thread::sleep_for(200ms);
            // Set the value and notify
            prom.set_value_at_thread_exit(42);
        }).detach(); // note : dangerous but safe in this case

        // ... Do something in the main thread ...
        std::this_thread::sleep_for(200ms);

        // Wait and get the result of the asynchronous computation
        int result = fut.get();

        REQUIRE(result == 42);
    }

    TEST_CASE("get a future from a packaged task executed on a separate thread", "[future]")
    {
        // Wrap a long computation in a packaged_task
        std::packaged_task<int()> task([] {
            // ... Long computation ...
            std::this_thread::sleep_for(200ms);
            return 42;
        });

        // Get the associated future
        std::future<int> fut = task.get_future();

        // Launch the thread that performs the long computation asynchronously
        std::thread(std::move(task)).detach(); // note : dangerous

        // ... Do something in the main thread ...
        std::this_thread::sleep_for(200ms);

        // Wait and get the result of the asynchronous computation
        int result = fut.get();

        REQUIRE(result == 42);
    }

    TEST_CASE("get a future from an asynchronous computation", "[future]")
    {
        // In one instruction:
        // - Launch or re-use a thread that performs a long computation asynchronously
        // - Wrap this computation in a lambda
        // - Get the associated future
        std::future<int> fut = std::async(std::launch::async, [] {
            // ... Long computation ...
            std::this_thread::sleep_for(200ms);
            return 42;
        });

        // ... Do something in the main thread ...
        std::this_thread::sleep_for(200ms);

        // Wait and get the result of the asynchronous computation
        int result = fut.get();

        REQUIRE(result == 42);
    }

    TEST_CASE("get a shared future from an asynchronous computation", "[future]")
    {
        // Launch an asynchronous computation
        std::future<int> fut = std::async(std::launch::async, [] {
            // ... Long computation ...
            std::this_thread::sleep_for(200ms);
            return 42;
        });

        // Create a shared future from the initial non-shared future
        // Note : the initial future is detached from its promise, and must not be used afterward
        std::shared_future<int> sh_fut = std::move(fut); // Note : std::move

        static constexpr std::size_t other_threads_count = 3;
        std::array<int, other_threads_count> other_threads_future_values{};
        std::array<std::thread, other_threads_count> other_threads{};

        // Note : capture sh_fut by copy
        auto thread_func = [&other_threads_future_values, sh_fut](std::size_t index) {
            other_threads_future_values[index] = sh_fut.get();
        };

        // Launch three threads, each waiting for the asynchronous computation
        for (std::size_t index = 0; index != other_threads_count; ++index)
            other_threads[index] = std::thread{thread_func, index};

        // Wait for the three threads to terminate
        for (auto& th : other_threads)
            th.join();

        // Each other thread has received the value
        for (auto other_thread_value : other_threads_future_values)
            REQUIRE(other_thread_value == 42);

        // Wait and get the result of the asynchronous computation
        int result = sh_fut.get();

        REQUIRE(result == 42);
    } // namespace

    // Each thread has its own global variable with the same name
    thread_local int per_thread_global_variable{123};

    TEST_CASE("thread_local", "[threads]")
    {
        // Create 3 threads that will modify their own variable's value
        static constexpr std::size_t other_threads_count = 3;
        std::array<int, other_threads_count> other_threads_variables_values{};
        std::array<std::thread, other_threads_count> other_threads{};

        auto thread_func = [&](std::size_t index) {
            // Note : each thread can safely modify its own variable
            per_thread_global_variable = static_cast<int>(index);
            other_threads_variables_values[index] = per_thread_global_variable;
        };

        // Launch the threads
        for (std::size_t index = 0; index != other_threads_count; ++index)
            other_threads[index] = std::thread{thread_func, index};

        // Wait for them to finish
        for (auto& other_thread : other_threads)
            other_thread.join();

        // Each other thread has modified its own variable's value
        for (std::size_t index = 0; index != other_threads_count; ++index)
            REQUIRE(other_threads_variables_values[index] == static_cast<int>(index));

        // The main thread's variable value was not modified
        REQUIRE(per_thread_global_variable == 123);
    }

    TEST_CASE("mutex & condition variables", "[threads]")
    {
        // Create a queue
        std::deque<int> queue{};
        std::condition_variable cv{};
        std::mutex mut{};

        // Create a producer thread
        std::thread producer{[&] {
            for (int i = 1; i != 5; ++i)
            {
                // Simulate a lenghty computation
                std::this_thread::sleep_for(100ms);

                // Produce
                {
                    std::lock_guard lock{mut};
                    queue.push_back(10 * i);
                }

                // Notify
                cv.notify_one();
            }

            {
                std::lock_guard lock{mut};
                queue.push_back(0); // Send special value meaning "stop"
            }
            cv.notify_one();
        }};

        // Create a consumer thread
        std::vector<int> received_data{};
        std::thread consumer{[&] {
            std::unique_lock lock{mut};
            for (;;)
            {
                cv.wait(lock, [&] { return !queue.empty(); });
                assert(!queue.empty());
                const auto data = queue.front();
                if (data == 0)
                    return;
                queue.pop_front();
                received_data.push_back(data);
            }
        }};

        producer.join();
        consumer.join();

        REQUIRE(received_data == std::vector{10, 20, 30, 40});
    }

    TEST_CASE("atomic", "[threads]")
    {
        std::atomic<bool> stop_thread{false};

        // Launch a thread which waits for the atomic variable to become true
        std::thread th{[&] {
            do
            {
                // Do some work repetitively
                std::this_thread::sleep_for(100ms);
                // Regularly check if we must stop
            } while (!stop_thread);
        }};

        std::this_thread::sleep_for(500ms);

        // Tell the thread to stop
        stop_thread = true;

        // Wait for the thread to stop
        th.join();
    }

} // namespace
