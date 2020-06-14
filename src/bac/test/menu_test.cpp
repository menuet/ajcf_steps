
#include <bac/util/menu.hpp>
#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

TEST_CASE("TEST bac::menu_loop")
{
    // ARRANGE
    std::vector<int> selected_options{};
    bac::Menu menu{{'0', "Quit",
                    [&] {
                        selected_options.push_back(0);
                        return bac::QuitMenu::Yes;
                    }},
                   {'1', "Play",
                    [&] {
                        selected_options.push_back(1);
                        return bac::QuitMenu::No;
                    }},
                   {'2', "Surf", [&] {
                        selected_options.push_back(2);
                        return bac::QuitMenu::No;
                    }}};
    std::stringstream out;
    std::stringstream in{"2\nXXX\n1\n0\n"};

    // ACT
    bac::menu_loop([](std::ostream& out) { out << "Hello"; }, out, in, menu);

    // ASSERT
    const std::string output_result = out.str();
    constexpr std::string_view expected_output =
        "Hello\n"
        "0 -> Quit\n"
        "1 -> Play\n"
        "2 -> Surf\n"
        "Hello\n"
        "0 -> Quit\n"
        "1 -> Play\n"
        "2 -> Surf\n"
        "Please enter a valid choice\n"
        "Hello\n"
        "0 -> Quit\n"
        "1 -> Play\n"
        "2 -> Surf\n"
        "Hello\n"
        "0 -> Quit\n"
        "1 -> Play\n"
        "2 -> Surf\n";
    REQUIRE(output_result == expected_output);
    REQUIRE(selected_options == (std::vector{2, 1, 0}));
}
