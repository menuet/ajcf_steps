
#include <bac/domain/board.hpp>
#include <bac/domain/codebreaker.hpp>
#include <bac/domain/options.hpp>
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("TEST bac::ask_attempt WHEN attempt is invalid and then is valid")
{
    // ARRANGE
    const bac::Options options{};
    bac::HumanCodebreaker codebreaker{};
    std::stringstream out{};
    std::stringstream in{"AAB\nMLKQSD\nABCDE"};

    // ASK
    const bac::Code attempt = codebreaker.ask_attempt(out, in, options, std::vector<bac::AttemptAndFeedback>{});

    // ASSERT
    REQUIRE(attempt.value == "ABCDE");
    REQUIRE(out.str() ==
            "Hey, codebreaker!\n"
            "What is your guess #01 (5 characters between 'A' and 'H')\n"
            "? "
            "Your guess has an invalid length or contains non-allowed characters, please try again\n"
            "What is your guess #01 (5 characters between 'A' and 'H')\n"
            "? "
            "Your guess has an invalid length or contains non-allowed characters, please try again\n"
            "What is your guess #01 (5 characters between 'A' and 'H')\n"
            "? ");
}
