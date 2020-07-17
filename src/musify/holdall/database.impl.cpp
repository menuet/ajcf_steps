
#include "database.impl.hpp"
#include "input_output.impl.hpp"
#include <exception>
#include <fstream>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines(std::ostream& output_stream, std::istream& input_stream)
    {
        std::vector<std::string> database_lines{};

        bool add_another_line = io::ask_question_get_yesno(output_stream, input_stream,
                                                           "Would you like to add another line to the database (Y/N)");

        while (add_another_line)
        {
            const std::string database_line = io::ask_question_get_string(
                output_stream, input_stream, "What is the new database line (please follow the database syntax)");
            database_lines.push_back(database_line);

            add_another_line = io::ask_question_get_yesno(output_stream, input_stream,
                                                          "Would you like to add another line to the database (Y/N)");
        }

        return database_lines;
    }

    std::pair<std::string, std::string> parse_until(const std::string& text, char separator)
    {
        const auto position_separator = text.find(separator);
        if (position_separator == std::string::npos)
            return {text, std::string{}};

        const auto text_before_separator = text.substr(0, position_separator);
        const auto text_after_separator = text.substr(position_separator + 1);
        return {text_before_separator, text_after_separator};
    }

    std::optional<LoadingError> parse_and_load_database_line(const std::string& line, MutableDatabase& database)
    {
        if (line.empty())
            return std::nullopt;

        if (line[0] == '#')
            return std::nullopt;

        const auto [line_type, remaining_text] = parse_until(line, '=');

        if (line_type == "Artist")
        {
            return parse_and_load_artist(remaining_text, database);
        }

        if (line_type == "Album")
        {
            return parse_and_load_album(remaining_text, database);
        }

        if (line_type == "Song")
        {
            return parse_and_load_song(remaining_text, database);
        }

        return LoadingError::UnknownLineType;
    }

    template <typename K, typename T>
    typename BintreeOrHashtable<K, T>::mapped_type* find(BintreeOrHashtable<K, T>& map, const std::string& name)
    {
        const auto iter = map.find(name);
        if (iter == map.end())
            return nullptr;
        auto& obj = iter->second;
        return &obj;
    }

    template <typename T>
    typename std::vector<T>::value_type* find(std::vector<T>& vec, const std::string& name)
    {
        for (auto& obj : vec)
        {
            if (obj.name == name)
                return &obj;
        }
        return nullptr;
    }

    std::optional<LoadingError> parse_and_load_artist(std::string name_year_rating_genre, MutableDatabase& database)
    {
        const auto [name, year_rating_genre] = parse_until(name_year_rating_genre, ',');
        if (name.empty())
            return LoadingError::IncompleteLine;
        if (find(database.artists, name))
            return LoadingError::DuplicateArtist;
        const auto [year, rating_genre] = parse_until(year_rating_genre, ',');
        if (year.empty())
            return LoadingError::IncompleteLine;
        const auto [rating, genre] = parse_until(rating_genre, ',');
        if (rating.empty() || genre.empty())
            return LoadingError::IncompleteLine;
        database.artists.insert({name, {name, year, rating, genre, {}}});
        return std::nullopt;
    }

    std::optional<LoadingError> parse_and_load_album(std::string name_artistname_date, MutableDatabase& database)
    {
        const auto [name, artistname_date] = parse_until(name_artistname_date, ',');
        if (name.empty())
            return LoadingError::IncompleteLine;
        if (find(database.albums, name))
            return LoadingError::DuplicateAlbum;
        const auto [artistname, date] = parse_until(artistname_date, ',');
        if (artistname.empty() || date.empty())
            return LoadingError::IncompleteLine;
        const auto artist = find(database.artists, artistname);
        if (!artist)
            return LoadingError::UnknownArtist;
        const auto iter_and_result = database.albums.insert({name, {name, artist, date}});
        const auto& album = iter_and_result.first->second;
        Artist* mutable_artist = const_cast<Artist*>(artist);
        mutable_artist->albums.push_back(&album);
        return {};
    }

    std::optional<LoadingError> parse_and_load_song(std::string name_albumname_artistname_duration,
                                                    MutableDatabase& database)
    {
        const auto [name, albumname_artistname_duration] = parse_until(name_albumname_artistname_duration, ',');
        if (name.empty())
            return LoadingError::IncompleteLine;
        if (find(database.songs, name))
            return LoadingError::DuplicateSong;
        const auto [albumname, artistname_duration] = parse_until(albumname_artistname_duration, ',');
        if (albumname.empty())
            return LoadingError::IncompleteLine;
        const auto album = find(database.albums, albumname);
        if (!album)
            return LoadingError::UnknownAlbum;
        const auto [artistname, duration] = parse_until(artistname_duration, ',');
        if (artistname.empty() || duration.empty())
            return LoadingError::IncompleteLine;
        const auto artist = find(database.artists, artistname);
        if (!artist)
            return LoadingError::UnknownArtist;
        Song song{};
        song.name = name;
        song.album = album;
        song.artist = artist;
        song.duration = duration;
        database.songs.push_back(song);
        return {};
    }

    bool operator==(const Artist& artist1, const Artist& artist2)
    {
        return artist1.name == artist2.name && artist1.start_year == artist2.start_year &&
               artist1.rating == artist2.rating && artist1.genre == artist2.genre;
    }

    bool operator==(const Album& album1, const Album& album2)
    {
        return album1.name == album2.name && album1.artist == album2.artist && album1.date == album2.date;
    }

    bool operator==(const Song& song1, const Song& song2)
    {
        return song1.name == song2.name && song1.album == song2.album && song1.artist == song2.artist &&
               song1.duration == song2.duration;
    }

}} // namespace musify::database
