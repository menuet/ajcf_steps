
#include "strong_types.hpp"
#include <fmt/format.h>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <string>

namespace musify { namespace strong {

    std::ostream& operator<<(std::ostream& output_stream, Year year)
    {
        output_stream << static_cast<int>(year.value);
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, Rating rating)
    {
        fmt::format_to(std::ostream_iterator<char>(output_stream), "{:.1f}", static_cast<float>(rating));
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, Genre genre)
    {
        switch (genre)
        {
        case Genre::Pop:
            output_stream << "Pop";
            break;
        case Genre::Rock:
            output_stream << "Rock";
            break;
        case Genre::Rap:
            output_stream << "Rap";
            break;
        case Genre::Jazz:
            output_stream << "Jazz";
            break;
        case Genre::Unknown:
            output_stream << "Unknown";
            break;
        }
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, Date date)
    {
        fmt::format_to(std::ostream_iterator<char>(output_stream), "{:04}-{:02}-{:02}",
                       static_cast<int>(date.value.year()), static_cast<unsigned>(date.value.month()),
                       static_cast<unsigned>(date.value.day()));
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, Duration duration)
    {
        const auto full_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration.value());
        const auto remaining_seconds = duration.value() - full_minutes;
        fmt::format_to(std::ostream_iterator<char>(output_stream), "{:02}m{:02}s", full_minutes.count(),
                       remaining_seconds.count());
        return output_stream;
    }

    std::optional<Year> parse_year(std::string_view text)
    {
        if (text.length() != 4)
            return std::nullopt;
        bool all_digits = std::all_of(text.begin(), text.end(), [](const char c) { return std::isdigit(c); });
        if (!all_digits)
            return std::nullopt;
        int year{};
        const std::from_chars_result result = std::from_chars(text.data(), text.data() + text.length(), year);
        if (std::make_error_condition(result.ec) || result.ptr != text.data() + text.length())
            return std::nullopt;
        return Year{date::year{year}};
    }

    std::optional<Rating> parse_rating(std::string_view text)
    {
        float rating{};
#ifdef __cpp_lib_to_chars
        const std::from_chars_result result = std::from_chars(text.data(), text.data() + text.length(), rating);
        if (std::make_error_condition(result.ec) || result.ptr != text.data() + text.length())
            return std::nullopt;
#else
        rating = std::stof(std::string{text});
#endif
        if (rating < 0.0f || rating > 5.0f)
            return std::nullopt;
        return Rating{rating};
    }

    std::optional<Genre> parse_genre(std::string_view text)
    {
        if (text == "Pop")
            return Genre::Pop;
        if (text == "Rock")
            return Genre::Rock;
        if (text == "Rap")
            return Genre::Rap;
        if (text == "Jazz")
            return Genre::Jazz;
        if (text == "Unknown")
            return Genre::Unknown;
        return std::nullopt;
    }

    std::optional<Date> parse_date([[maybe_unused]] std::string_view text)
    {
        date::sys_days date_point{};
        std::istringstream iss{std::string{text}};
        iss >> date::parse("%Y/%m/%d", date_point);
        if (!iss)
            return std::nullopt;
        return Date{date::year_month_day{date_point}};
    }

    std::optional<Duration> parse_duration([[maybe_unused]] std::string_view text)
    {
#if 0 // FIX ME
        std::chrono::system_clock::time_point time_point{};
        std::istringstream iss{std::string{text}};
        iss >> date::parse("%M:%S", time_point);
        if (!iss)
            return std::nullopt;
        return Duration{std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch())};
#else
        int min{};
        int sec{};
        const auto result = std::sscanf(std::string{text}.c_str(), "%d:%d", &min, &sec);
        if (result != 2)
            return std::nullopt;
        return Duration{std::chrono::minutes{min} + std::chrono::seconds{sec}};
#endif
    }

}} // namespace musify::strong
