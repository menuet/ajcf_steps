
#include <bac/util/input.hpp>
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("TEST bac::ask_string")
{
    // ARRANGE
    std::stringstream in{"something that the user has typed"};

    // ACT
    const std::string result = bac::ask_string(in);

    // ASSERT
    REQUIRE(result == "something that the user has typed");
}

TEST_CASE("TEST bac::ask<int> WHEN user does not type an integer number")
{
    // ARRANGE
    std::stringstream in{"something that is not a number"};

    // ACT
    const auto result_opt = bac::ask<int>(in);

    // ASSERT
    REQUIRE(!result_opt);
}

TEST_CASE("TEST bac::ask<int> WHEN user types an integer number")
{
    // ARRANGE
    std::stringstream in{"42"};

    // ACT
    const auto result_opt = bac::ask<int>(in);

    // ASSERT
    REQUIRE((result_opt && *result_opt == 42));
}

TEST_CASE("TEST bac::ask<char> WHEN user does not type a character")
{
    // ARRANGE
    std::stringstream in{"something that is not a single character"};

    // ACT
    const auto result_opt = bac::ask<char>(in);

    // ASSERT
    REQUIRE(!result_opt);
}

TEST_CASE("TEST bac::ask<char> WHEN user types an integer number")
{
    // ARRANGE
    std::stringstream in{"Z"};

    // ACT
    const auto result_opt = bac::ask<char>(in);

    // ASSERT
    REQUIRE((result_opt && *result_opt == 'Z'));
}
