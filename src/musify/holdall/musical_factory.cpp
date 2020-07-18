
#include "musical_factory.hpp"
#include "musical_things.hpp"

namespace musify::database {

    std::unique_ptr<MusicalThing> MusicalFactory::create_artist(std::string name, strong::Year start_year,
                                                                strong::Rating rating, strong::Genre genre)
    {
        return std::make_unique<Artist>(std::move(name), start_year, rating, genre);
    }

    std::unique_ptr<MusicalThing> MusicalFactory::create_album(std::string name, std::string artist_name,
                                                               strong::Date date)
    {
        return std::make_unique<Album>(std::move(name), std::move(artist_name), date);
    }

    std::unique_ptr<MusicalThing> MusicalFactory::create_song(std::string name, std::string album_name,
                                                              std::string artist_name, strong::Duration duration)
    {
        return std::make_unique<Song>(std::move(name), std::move(album_name), std::move(artist_name), duration);
    }

} // namespace musify::database
