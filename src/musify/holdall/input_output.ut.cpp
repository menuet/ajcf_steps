
#include "input_output.impl.hpp"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

namespace musify { namespace io {

    TEST_CASE("musify::io::ask_question_get_string test", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"Donald"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_string(output_stream, input_stream, "What is your name");

        // ASSERT
        REQUIRE(result == "Donald");
        REQUIRE(output_stream.str() == "What is your name ? ");
    }

    TEST_CASE("musify::io::ask_question_get_char test", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"F"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_char(output_stream, input_stream, "What is your gender");

        // ASSERT
        REQUIRE(result == 'F');
        REQUIRE(output_stream.str() == "What is your gender ? ");
    }

    TEST_CASE("musify::io::ask_question_get_int test", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"123"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_int(output_stream, input_stream, "What is your age");

        // ASSERT
        REQUIRE(result == 123);
        REQUIRE(output_stream.str() == "What is your age ? ");
    }

    TEST_CASE("musify::io::ask_question_get_double test", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"1.75"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_double(output_stream, input_stream, "What is your size");

        // ASSERT
        REQUIRE(result == Approx{1.75});
        REQUIRE(output_stream.str() == "What is your size ? ");
    }

}} // namespace musify::io
