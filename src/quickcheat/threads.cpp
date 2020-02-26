
// https://en.cppreference.com/w/cpp/header/thread
// https://en.cppreference.com/w/cpp/header/mutex

#include <catch2/catch.hpp>
#include <chrono>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std::literals;

namespace {

    void log_with_synchronization(const std::string& text, std::mutex& mut, std::stringstream& output)
    {
        // guard's constructor calls mut.lock()
        std::lock_guard guard{mut};

        output << "[" << std::this_thread::get_id() << "] : " << text << "\n";

    } // guard's destructor calls mut.lock()

    std::string start_a_thread_then_wait_for_it_to_terminate()
    {
        std::mutex mut;
        std::stringstream output;
        int computation_result = 0;

        std::thread th([&computation_result, &mut, &output] {
            log_with_synchronization("Starting thread th", mut, output);

            // simulation d'un long calcul
            for (int i = 0; i < 3; ++i)
            {
                log_with_synchronization("Thinking...", mut, output);
                std::this_thread::sleep_for(1s);
            }

            // non need for synchronization because th and the main thread
            // never access computation_result at the tame time
            computation_result = 42;

            log_with_synchronization("Stopping thread th", mut, output);
        });

        log_with_synchronization("While the other thread is working, please wait...", mut, output);
        for (int i = 0; i < 20; ++i)
        {
            log_with_synchronization(".", mut, output);

            std::this_thread::sleep_for(200ms);
        }

        // Wait for the thread th to finish
        th.join();

        // No need for synchronization, we know that thread th is terminated
        output << "\nComputation result : " << computation_result << "\n";

        return output.str();
    }

    TEST_CASE("threads manipulation", "[threads][!hide]")
    {
        const auto output = start_a_thread_then_wait_for_it_to_terminate();

        REQUIRE(output.find("Thinking") != std::string::npos);
    }

    void launch_a_thread_and_dont_wait_for_it_to_finish_OK_BUT_VERY_DANGEROUS()
    {
        int local_variable_in_the_main_thread_stack = 5;

        std::thread th([&local_variable_in_the_main_thread_stack] {
            std::this_thread::sleep_for(3s);
            local_variable_in_the_main_thread_stack = 42; // UB !!!
        });

        // detach the variable th from the underlying OS thread
        th.detach();
    } // the main thread exist from this function
    // the local variable is destroyed,
    // but the launched thread will access the destroyed memory zone in 3 seconds!!!

    void launch_a_thread_and_dont_wait_for_it_to_finish_and_dont_detach_it_FORBIDDEN()
    {
        std::thread th([] { std::this_thread::sleep_for(3s); });

    } // th's destructor calls std::terminate because it is not detached

} // namespace
