
#include "musical_things.hpp"
#include <iostream>

namespace musify::database {

    void Artist::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_start_year << ", " << m_rating << ", " << m_genre;
    }

    void Album::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_artist_name << ", " << m_date;
    }

    void Song::details_to_stream(std::ostream& output_stream) const
    {
        output_stream << m_album_name << ", " << m_artist_name << ", " << m_duration;
    }

} // namespace musify::database
