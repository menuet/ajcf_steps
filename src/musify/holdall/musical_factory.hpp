
#pragma once

#include "strong_types.hpp"
#include <memory>
#include <string>

namespace musify::database {

    class MusicalThing;

    class MusicalFactory
    {
    public:
        std::unique_ptr<MusicalThing> create_artist(std::string name, strong::Year start_year, strong::Rating rating,
                                                    strong::Genre genre);

        std::unique_ptr<MusicalThing> create_album(std::string name, std::string artist_name, strong::Date date);

        std::unique_ptr<MusicalThing> create_song(std::string name, std::string album_name, std::string artist_name,
                                                  strong::Duration duration);
    };

} // namespace musify::database
