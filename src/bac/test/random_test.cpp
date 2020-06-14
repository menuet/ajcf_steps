
#include <bac/util/random.hpp>
#include <catch2/catch.hpp>

TEST_CASE("TEST bac::generate_random_value_between of int")
{
    for (unsigned int i = 0U; i != 1000; ++i)
    {
        // ACT
        const auto number = bac::generate_random_value_between(-15, 42);

        // ASSERT
        REQUIRE(number >= -15);
        REQUIRE(number <= 42);
    }
}

TEST_CASE("TEST bac::generate_random_value_between of char")
{
    for (unsigned int i = 0U; i != 1000; ++i)
    {
        // ACT
        const auto character = bac::generate_random_value_between('A', 'E');

        // ASSERT
        REQUIRE(character >= 'A');
        REQUIRE(character <= 'E');
    }
}
