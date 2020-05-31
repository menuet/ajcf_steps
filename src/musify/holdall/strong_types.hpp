
#pragma once

#include <string_view>
#include <date/date.h>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>

namespace musify { namespace strong {

    struct Year
    {
        date::year value{};
    };

    std::ostream& operator<<(std::ostream& output_stream, Year year);

    class Rating
    {
    public:
        Rating() = default;

        explicit Rating(float value) : m_value{value}
        {
            assert(value >= 0.0f && value <= 5.0f);
        }

        explicit operator float() const
        {
            return m_value;
        }

        friend bool operator==(const Rating& left, const Rating& right)
        {
            return std::abs(left.m_value - right.m_value) < 0.1f;
        }

        friend bool operator!=(const Rating& left, const Rating& right)
        {
            return !(left == right);
        }

    private:
        float m_value{};
    };

    std::ostream& operator<<(std::ostream& output_stream, Rating rating);

    enum class Genre
    {
        Unknown,
        Pop,
        Rock,
        Rap,
        Jazz,
    };

    std::ostream& operator<<(std::ostream& output_stream, Genre genre);

    struct Date
    {
        date::year_month_day value{};
    };

    std::ostream& operator<<(std::ostream& output_stream, Date date);

    class Duration
    {
    public:
        Duration() = default;

        explicit Duration(std::chrono::seconds value) : m_value{value}
        {
            assert(value.count() > 0);
        }

        std::chrono::seconds value() const
        {
            return m_value;
        }

        friend bool operator==(const Duration& left, const Duration& right)
        {
            return left.m_value == right.m_value;
        }

        friend bool operator!=(const Duration& left, const Duration& right)
        {
            return !(left == right);
        }

    private:
        std::chrono::seconds m_value{};
    };

    std::ostream& operator<<(std::ostream& output_stream, Duration duration);

    std::optional<Year> parse_year(std::string_view text);

    std::optional<Rating> parse_rating(std::string_view text);

    std::optional<Genre> parse_genre(std::string_view text);

    std::optional<Date> parse_date(std::string_view text);

    std::optional<Duration> parse_duration(std::string_view text);

}} // namespace musify::strong
