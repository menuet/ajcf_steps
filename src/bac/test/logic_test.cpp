
#include <bac/domain/board.hpp>
#include <bac/domain/logic.hpp>
#include <bac/domain/options.hpp>
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("TEST bac::generate_random_secret_code")
{
    // ARRANGE
    const bac::Options options{};

    // ACT
    const auto code = bac::generate_random_secret_code(options);

    // ASSERT
    REQUIRE(code.value.size() == options.number_of_characters_per_code);
    for (const char c : code.value)
    {
        REQUIRE(c >= options.minimum_allowed_character);
        REQUIRE(c <= options.maximum_allowed_character);
    }
}

TEST_CASE("TEST bac::validate_attempt WHEN attempt's length is too small")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Code attempt{};

    // ACT
    const bool result = bac::validate_attempt(options, attempt);

    // ASSERT
    REQUIRE(!result);
}

TEST_CASE("TEST bac::validate_attempt WHEN attempt contains non-allowed characters")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Code attempt{"ZZZZZ"};

    // ACT
    const bool result = bac::validate_attempt(options, attempt);

    // ASSERT
    REQUIRE(!result);
}

TEST_CASE("TEST bac::compare_attempt_with_secret_code WHEN 0 bull and 3 cows")
{
    // ARRANGE
    const bac::Code attempt{std::string{"ABEDC"}};
    const bac::Code secret{std::string{"CCCAB"}};

    // ACT
    const bac::Feedback feedback = bac::compare_attempt_with_secret_code(attempt, secret);

    // ASSERT
    REQUIRE(feedback.bulls == 0);
    REQUIRE(feedback.cows == 3);
}

TEST_CASE("TEST bac::compare_attempt_with_secret_code WHEN 1 bull and 2 cows")
{
    // ARRANGE
    const bac::Code attempt{std::string{"ABCDE"}};
    const bac::Code secret{std::string{"CCCAB"}};

    // ACT
    const bac::Feedback feedback = bac::compare_attempt_with_secret_code(attempt, secret);

    // ASSERT
    REQUIRE(feedback.bulls == 1);
    REQUIRE(feedback.cows == 2);
}

TEST_CASE("TEST bac::compare_attempt_with_secret_code WHEN 1 bull and 0 cow")
{
    // ARRANGE
    const bac::Code attempt{std::string{"AAAAA"}};
    const bac::Code secret{std::string{"CCCAB"}};

    // ACT
    const bac::Feedback feedback = bac::compare_attempt_with_secret_code(attempt, secret);

    // ASSERT
    REQUIRE(feedback.bulls == 1);
    REQUIRE(feedback.cows == 0);
}

TEST_CASE("TEST bac::compare_attempt_with_secret_code WHEN 5 bulls and 0 cow")
{
    // ARRANGE
    const bac::Code attempt{std::string{"CCCAB"}};
    const bac::Code secret{std::string{"CCCAB"}};

    // ACT
    const bac::Feedback feedback = bac::compare_attempt_with_secret_code(attempt, secret);

    // ASSERT
    REQUIRE(feedback.bulls == 5);
    REQUIRE(feedback.cows == 0);
}

TEST_CASE("TEST bac::compare_attempt_with_secret_code WHEN 0 bulls and 0 cows")
{
    // ARRANGE
    const bac::Code attempt{"ABCAB"};
    const bac::Code secret_code{"DEDED"};

    // ACT
    const bac::Feedback feedback = bac::compare_attempt_with_secret_code(attempt, secret_code);

    // ASSERT
    REQUIRE(feedback.bulls == 0);
    REQUIRE(feedback.cows == 0);
}

TEST_CASE("TEST bac::has_codemaker_won WHEN codemaker is not yet winning")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Board board{bac::Code{"ABCDE"}};

    // ACT
    const bool result = bac::has_codemaker_won(options, board);

    // ASSERT
    REQUIRE(!result);
}

TEST_CASE("TEST bac::has_codebreaker_won WHEN codebreaker is not yet winning")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Board board{bac::Code{"ABCDE"}};

    // ACT
    const bool result = bac::has_codebreaker_won(options, board);

    // ASSERT
    REQUIRE(!result);
}
