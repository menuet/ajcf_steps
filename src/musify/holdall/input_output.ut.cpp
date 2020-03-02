
#include "input_output.impl.hpp"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

namespace musify { namespace io {

    TEST_CASE("TEST musify::io::ask_question_get_string WITH simple word", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"Donald"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_string(output_stream, input_stream, "What is your name");

        // ASSERT
        REQUIRE(result == "Donald");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your name ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_string WITH empty text", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{""};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_string(output_stream, input_stream, "What is your name");

        // ASSERT
        REQUIRE(result == "");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your name ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_string WITH text with space", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"Donald Trump"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_string(output_stream, input_stream, "What is your name");

        // ASSERT
        REQUIRE(result == "Donald Trump");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your name ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_char WITH one character", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"F"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_char(output_stream, input_stream, "What is your gender");

        // ASSERT
        REQUIRE(result == 'F');
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your gender ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_char WITH several characters", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"abcd"};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result =
                ask_question_get_char(output_stream, input_stream, "What is your gender");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input does not contain exactly one character");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your gender ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_char WITH empty text", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{""};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result =
                ask_question_get_char(output_stream, input_stream, "What is your gender");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input does not contain exactly one character");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your gender ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_int WITH an integer number", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"123"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_int(output_stream, input_stream, "What is your age");

        // ASSERT
        REQUIRE(result == 123);
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your age ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_int WITH an integer number followed by anything", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"456abc"};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result = ask_question_get_int(output_stream, input_stream, "What is your age");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input is empty or contains some non-numeric characters");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your age ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_int WITH empty text", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{""};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result = ask_question_get_int(output_stream, input_stream, "What is your age");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input is empty or contains some non-numeric characters");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your age ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_double WITH a real number", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"1.75"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_double(output_stream, input_stream, "What is your size");

        // ASSERT
        REQUIRE(result == Approx{1.75});
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your size ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_double WITH a real number followed by anything", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"1.75;qdsf"};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result =
                ask_question_get_double(output_stream, input_stream, "What is your size");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input is empty or contains some non-numeric characters");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your size ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_double WITH empty text", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{""};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result =
                ask_question_get_double(output_stream, input_stream, "What is your size");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input is empty or contains some non-numeric characters");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "What is your size ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_yesno WITH Y", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"Y"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_yesno(output_stream, input_stream, "Do you like C++");

        // ASSERT
        REQUIRE(result);
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "Do you like C++ ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_yesno WITH N", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"N"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_question_get_yesno(output_stream, input_stream, "Do you like C++");

        // ASSERT
        REQUIRE(!result);
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "Do you like C++ ? ");
    }

    TEST_CASE("TEST musify::io::ask_question_get_yesno WITH some dummy char", "[io]")
    {
        // ARRANGE
        std::stringstream input_stream{"O"};
        std::stringstream output_stream;

        // ACT
        std::string exception_text{};
        try
        {
            [[maybe_unused]] const auto result = ask_question_get_yesno(output_stream, input_stream, "Do you like C++");
        }
        catch (std::domain_error& exception)
        {
            exception_text = exception.what();
        }

        // ASSERT
        REQUIRE(exception_text == "Your input must be Y (yes) or N (no)");
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() == "Do you like C++ ? ");
    }

}} // namespace musify::io
