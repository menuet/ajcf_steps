
#include <bac/domain/solver.hpp>
#include <catch2/catch.hpp>
#include <algorithm>
#include <numeric>

static unsigned int power(unsigned int number, unsigned int exponent)
{
    int power_of_number = 1;
    for (unsigned int index = 0U; index != exponent; ++index)
        power_of_number *= number;
    return power_of_number;
}

static unsigned int computed_value(const bac::Options options, const bac::Code& code)
{
    const unsigned int characters_range_size =
        static_cast<unsigned char>(options.maximum_allowed_character - options.minimum_allowed_character + 1);
    unsigned int value = 0U;
    unsigned int exp = 1U;
    for (char c : code.value)
    {
        value += static_cast<unsigned int>(c - options.minimum_allowed_character) * exp;
        exp *= characters_range_size;
    }
    return value;
}

TEST_CASE("TEST bac::generate_all_possible_codes")
{
    // ARRANGE
    bac::Options options;
    options.minimum_allowed_character = 'A';
    options.maximum_allowed_character = 'C';
    options.number_of_characters_per_code = 5;

    // ACT
    const auto possible_solutions = bac::generate_all_possible_codes(options);

    // ASSERT
    const auto expected_number_of_possible_codes =
        power(1 + options.maximum_allowed_character - options.minimum_allowed_character,
              options.number_of_characters_per_code);
    REQUIRE(possible_solutions.codes.size() == expected_number_of_possible_codes);
    auto iter = possible_solutions.codes.begin();
    auto value = computed_value(options, *iter);
    REQUIRE(value == 0);
    for (++iter; iter != possible_solutions.codes.end(); ++iter)
    {
        const auto next_value = computed_value(options, *iter);
        REQUIRE(next_value == value + 1);
        value = next_value;
    }
}

TEST_CASE("TEST bac::pick_random_attempt")
{
    // ARRANGE
    bac::PossibleCodes possible_solutions{{{"ABCDE"}, {"DEFGH"}, {"HABCD"}}};

    // ACT
    const auto code = bac::pick_random_attempt(possible_solutions);

    // ASSERT
    REQUIRE((code.value == "ABCDE" || code.value == "DEFGH" || code.value == "HABCD"));
}

TEST_CASE("TEST bac::remove_incompatible_codes_from_possible_solutions")
{
    // ARRANGE
    bac::PossibleCodes possible_solutions{{{"ABCDE"}, {"DEFGH"}, {"HABCD"}}};
    const bac::AttemptAndFeedback attempt_and_feedback{{"DABEC"}, {2, 2}};

    // ACT
    bac::remove_incompatible_codes_from_possible_solutions(attempt_and_feedback, possible_solutions);

    // ASSERT
    REQUIRE(possible_solutions.codes.size() == 1);
    REQUIRE(possible_solutions.codes[0].value == "HABCD");
}
