
#include <bac/domain/board.hpp>
#include <bac/domain/options.hpp>
#include <catch2/catch.hpp>
#include <sstream>

#if 0

TEST_CASE("TEST bac::display_board WHEN codebreaker has not yet played")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Board board{bac::Code{"ABCDE"}};
    std::stringstream out{};

    // ACT
    display_board(out, options, board, bac::DisplaySecretCode::No);

    // ASSERT
    const std::string output_result = out.str();
    REQUIRE(output_result ==
            "-------------------------------------\n"
            "| SECRET   * * * * * |              |\n"
            "-------------------------------------\n"
            "| ATTEMPTS           | BULLS | COWS |\n"
            "-------------------------------------\n"
            "| #12      . . . . . |       |      |\n"
            "| #11      . . . . . |       |      |\n"
            "| #10      . . . . . |       |      |\n"
            "| #09      . . . . . |       |      |\n"
            "| #08      . . . . . |       |      |\n"
            "| #07      . . . . . |       |      |\n"
            "| #06      . . . . . |       |      |\n"
            "| #05      . . . . . |       |      |\n"
            "| #04      . . . . . |       |      |\n"
            "| #03      . . . . . |       |      |\n"
            "| #02      . . . . . |       |      |\n"
            "| #01      . . . . . |       |      |\n"
            "-------------------------------------\n");
}

TEST_CASE("TEST bac::display_board WHEN codebreaker has given some attempts")
{
    // ARRANGE
    const bac::Options options{};
    const bac::Board board{{"ABCDE"},
                           {
                               {{"BCDEF"}, {0, 4}},
                               {{"GHFED"}, {0, 2}},
                               {{"AAAAA"}, {1, 0}},
                           }};
    std::stringstream out{};

    // ACT
    display_board(out, options, board, bac::DisplaySecretCode::Yes);

    // ASSERT
    const std::string output_result = out.str();
    REQUIRE(output_result ==
            "-------------------------------------\n"
            "| SECRET   A B C D E |              |\n"
            "-------------------------------------\n"
            "| ATTEMPTS           | BULLS | COWS |\n"
            "-------------------------------------\n"
            "| #12      . . . . . |       |      |\n"
            "| #11      . . . . . |       |      |\n"
            "| #10      . . . . . |       |      |\n"
            "| #09      . . . . . |       |      |\n"
            "| #08      . . . . . |       |      |\n"
            "| #07      . . . . . |       |      |\n"
            "| #06      . . . . . |       |      |\n"
            "| #05      . . . . . |       |      |\n"
            "| #04      . . . . . |       |      |\n"
            "| #03      A A A A A |   1   |  0   |\n"
            "| #02      G H F E D |   0   |  2   |\n"
            "| #01      B C D E F |   0   |  4   |\n"
            "-------------------------------------\n");
}

#endif
