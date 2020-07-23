
#include <bac/domain/options.hpp>
#include <bac/util/menu.hpp>
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("TEST bac::display_menu_and_configure")
{
    // ARRANGE
    std::stringstream out{};
    std::stringstream in{
        "3\n"
        "A\n"
        "0\n"};
    bac::Options options{};

    // ACT
    bac::display_menu_and_configure(out, in, options);

    // ASSERT
    const std::string output_result = out.str();
    constexpr std::string_view expected_result =
        "Here are the current options:\n"
        "Maximum number of attempts per game: 12\n"
        "Number of characters in a code: 5\n"
        "Range of allowed characters: from 'A' to 'H'\n"
        "Please select a menu option to configure the game options\n"
        "0 -> Back to main menu\n"
        "1 -> Modify Maximum number of attempts per game\n"
        "2 -> Modify Number of characters in a code\n"
        "3 -> Modify Minimum allowed character\n"
        "4 -> Modify Maximum allowed character\n"
        "5 -> Save options\n"
        "6 -> Load options\n"
        "Enter the Minimum allowed character: "
        "Here are the current options:\n"
        "Maximum number of attempts per game: 12\n"
        "Number of characters in a code: 5\n"
        "Range of allowed characters: from 'A' to 'H'\n"
        "Please select a menu option to configure the game options\n"
        "0 -> Back to main menu\n"
        "1 -> Modify Maximum number of attempts per game\n"
        "2 -> Modify Number of characters in a code\n"
        "3 -> Modify Minimum allowed character\n"
        "4 -> Modify Maximum allowed character\n"
        "5 -> Save options\n"
        "6 -> Load options\n";
    REQUIRE(output_result == expected_result);
}

TEST_CASE("TEST bac::save_options")
{
    // ARRANGE
    const bac::Options options{};
    std::stringstream out;

    // ACT
    const bool result = bac::save_options(out, options);

    // ASSERT
    REQUIRE(result);
    const std::string output_result = out.str();
    REQUIRE(output_result ==
            "max_number_of_attempts=12\n"
            "number_of_characters_per_code=5\n"
            "minimum_allowed_character=A\n"
            "maximum_allowed_character=H\n");
}

TEST_CASE("TEST bac::load_options")
{
    // ARRANGE
    bac::Options options{};
    std::stringstream in{
        "max_number_of_attempts=5\n"
        "number_of_characters_per_code=3\n"
        "minimum_allowed_character=1\n"
        "maximum_allowed_character=8\n"};

    // ACT
    const bool result = bac::load_options(in, options);

    // ASSERT
    REQUIRE(result);
    REQUIRE(options.max_number_of_attempts == 5);
    REQUIRE(options.number_of_characters_per_code == 3);
    REQUIRE(options.minimum_allowed_character == '1');
    REQUIRE(options.maximum_allowed_character == '8');
}
