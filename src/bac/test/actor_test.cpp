
#include <bac/util/actor.hpp>
#include <catch2/catch.hpp>
#include <atomic>
#include <chrono>

#if !defined(__GNUC__) || defined(__clang__)

TEST_CASE("TEST bac::Actor")
{
    // ARRANGE
    using namespace std::literals;
    std::vector<int> v;
    std::atomic<bool> done{false};
    bac::Actor actor;

    // ACT
    for (int i = 0; i != 5; ++i)
        actor.push_task([&, i] {
            v.push_back(i);
            if (v.size() == 5)
                done = true;
        });
    while (!done)
    {
        std::this_thread::sleep_for(10ms);
    }

    // ASSERT
    REQUIRE(v == (std::vector{0, 1, 2, 3, 4}));
}

TEST_CASE("TEST push_task with continuation with final get")
{
    // ARRANGE
    using namespace std::literals;
    std::vector<int> v;
    bac::Actor actor;

    // ACT
    auto cont = push_task(actor, [&] {
        v.push_back(1);
        return 42;
    });

    auto cont2 = cont.then(actor, [&](int i) {
        v.push_back(2);
        return 42 + i;
    });

    const auto result = cont2.get();

    // ASSERT
    REQUIRE(result == 42 + 42);
    REQUIRE(v == (std::vector{1, 2}));
}

TEST_CASE("TEST push_task with continuation with final wait")
{
    // ARRANGE
    using namespace std::literals;
    std::vector<int> v;
    bac::Actor actor;

    // ACT
    auto cont = push_task(actor, [&] {
        v.push_back(1);
        return 42;
    });

    int result{};
    auto cont2 = cont.then(actor, [&](int i) {
        v.push_back(2);
        result = i;
    });

    cont2.wait();

    // ASSERT
    REQUIRE(result == 42);
    REQUIRE(v == (std::vector{1, 2}));
}

#endif
