
#include "musical_things.hpp"
#include <iostream>

namespace musify::database {

    static std::pair<std::string_view, std::string_view> parse_until(std::string_view text, char separator)
    {
        const auto position_separator = text.find(separator);
        if (position_separator == std::string_view::npos)
            return {text, std::string_view{}};

        const auto text_before_separator = text.substr(0, position_separator);
        const auto text_after_separator = text.substr(position_separator + 1);
        return {text_before_separator, text_after_separator};
    }

    void Artist::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_start_year << ", " << m_rating << ", " << m_genre;
    }

    ParsingResult Artist::parse_details(std::string_view details)
    {
        const auto [year, rating_genre] = parse_until(details, ',');
        if (year.empty())
            return ParsingResult::IncompleteData;
        const auto [rating, genre] = parse_until(rating_genre, ',');
        if (rating.empty() || genre.empty())
            return ParsingResult::IncompleteData;
        const auto year_opt = strong::parse_year(year);
        if (!year_opt)
            return ParsingResult::InvalidData;
        const auto rating_opt = strong::parse_rating(rating);
        if (!rating_opt)
            return ParsingResult::InvalidData;
        const auto genre_opt = strong::parse_genre(genre);
        if (!genre_opt)
            return ParsingResult::InvalidData;
        m_start_year = year_opt.value();
        m_rating = rating_opt.value();
        m_genre = genre_opt.value();
        return ParsingResult::Ok;
    }

    void Album::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_artist_name << ", " << m_date;
    }

    ParsingResult Album::parse_details(std::string_view details)
    {
        const auto [artistname, date] = parse_until(details, ',');
        if (artistname.empty() || date.empty())
            return ParsingResult::IncompleteData;
        const auto date_opt = strong::parse_date(date);
        if (!date_opt)
            return ParsingResult::InvalidData;
        m_artist_name = artistname;
        m_date = date_opt.value();
        return ParsingResult::Ok;
    }

    void Song::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_album_name << ", " << m_artist_name << ", " << m_duration;
    }

    ParsingResult Song::parse_details(std::string_view details)
    {
        const auto [albumname, artistname_duration] = parse_until(details, ',');
        if (albumname.empty())
            return ParsingResult::IncompleteData;
        const auto [artistname, duration] = parse_until(artistname_duration, ',');
        if (artistname.empty() || duration.empty())
            return ParsingResult::IncompleteData;
        const auto duration_opt = strong::parse_duration(duration);
        if (!duration_opt)
            return ParsingResult::InvalidData;
        m_album_name = albumname;
        m_artist_name = artistname;
        m_duration = duration_opt.value();
        return ParsingResult::Ok;
    }

} // namespace musify::database
