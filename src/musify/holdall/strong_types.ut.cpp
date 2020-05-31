
#include "strong_types.hpp"
#include <catch2/catch.hpp>

namespace musify { namespace strong {

    using namespace std::literals;
    using namespace date::literals;

    TEST_CASE("TEST musify::strong::operator<< WITH year", "[strong]")
    {
        // ARRANGE
        std::stringstream output_stream;

        // ACT
        output_stream << Year{1984_y};

        // ASSERT
        REQUIRE(output_stream.good());
        REQUIRE(output_stream.str() == "1984");
    }

    TEST_CASE("TEST musify::strong::operator<< WITH rating", "[strong]")
    {
        // ARRANGE
        std::stringstream output_stream;

        // ACT
        output_stream << Rating{2.65f};

        // ASSERT
        REQUIRE(output_stream.good());
        REQUIRE(output_stream.str() == "2.7");
    }

    TEST_CASE("TEST musify::strong::operator<< WITH genre", "[strong]")
    {
        // ARRANGE
        std::stringstream output_stream;

        // ACT
        output_stream << Genre::Unknown;

        // ASSERT
        REQUIRE(output_stream.good());
        REQUIRE(output_stream.str() == "Unknown");
    }

    TEST_CASE("TEST musify::strong::operator<< WITH date", "[strong]")
    {
        // ARRANGE
        std::stringstream output_stream;

        // ACT
        output_stream << Date{jun / 1 / 2020};

        // ASSERT
        REQUIRE(output_stream.good());
        REQUIRE(output_stream.str() == "2020-06-01");
    }

    TEST_CASE("TEST musify::strong::operator<< WITH duration", "[strong]")
    {
        // ARRANGE
        std::stringstream output_stream;

        // ACT
        output_stream << Duration{3683s};

        // ASSERT
        REQUIRE(output_stream.good());
        REQUIRE(output_stream.str() == "61m23s");
    }

    TEST_CASE("TEST musify::strong::parse_year WITH valid year", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_year("1984");

        // ASSERT
        REQUIRE(result.has_value());
        REQUIRE(result.value().value == 1984_y);
    }

    TEST_CASE("TEST musify::strong::parse_year WITH alphanumeric mix", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_year("19A4");

        // ASSERT
        REQUIRE(!result.has_value());
    }

    TEST_CASE("TEST musify::strong::parse_year WITH too much characters", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_year("19846");

        // ASSERT
        REQUIRE(!result.has_value());
    }

    TEST_CASE("TEST musify::strong::parse_rating WITH valid rating", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_rating("3.5");

        // ASSERT
        REQUIRE(result.has_value());
        REQUIRE(static_cast<float>(result.value()) == Approx{3.5f});
    }

    TEST_CASE("TEST musify::strong::parse_genre WITH valid genre", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_genre("Pop");

        // ASSERT
        REQUIRE(result.has_value());
        REQUIRE(result.value() == Genre::Pop);
    }

    TEST_CASE("TEST musify::strong::parse_date WITH valid date YYYY/MM/DD", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_date("1782/08/04");

        // ASSERT
        REQUIRE(result.has_value());
        REQUIRE(result.value().value == 1782_y / aug / 4);
    }

    TEST_CASE("TEST musify::strong::parse_duration WITH valid duration MM:SS", "[strong]")
    {
        // ARRANGE

        // ACT
        const auto result = parse_duration("03:45");

        // ASSERT
        REQUIRE(result.has_value());
        REQUIRE(result.value().value() == (3min + 45s));
    }

}} // namespace musify::strong
