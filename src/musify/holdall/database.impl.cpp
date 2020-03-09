
#include "database.impl.hpp"
#include "database.hpp"
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

    LoadingResult parse_and_load_database_line(const std::string& line, Database& database)
    {
        if (line.empty())
            return LoadingResult::Ok;

        if (line[0] == '#')
            return LoadingResult::Ok;

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

        return LoadingResult::UnknownLineType;
    }

    LoadingResult parse_and_load_artist(std::string name_year_rating_genre, Database& database)
    {
        const auto [name, year_rating_genre] = parse_until(name_year_rating_genre, ',');
        if (name.empty())
            return LoadingResult::IncompleteLine;
        if (find_artist(database, name))
            return LoadingResult::DuplicateArtist;
        const auto [year, rating_genre] = parse_until(year_rating_genre, ',');
        if (year.empty())
            return LoadingResult::IncompleteLine;
        const auto [rating, genre] = parse_until(rating_genre, ',');
        if (rating.empty() || genre.empty())
            return LoadingResult::IncompleteLine;
        Artist artist{};
        artist.name = name;
        artist.start_year = year;
        artist.rating = rating;
        artist.genre = genre;
        database.artists.push_back(artist);
        return LoadingResult::Ok;
    }

    LoadingResult parse_and_load_album(std::string name_artistname_date, Database& database)
    {
        const auto [name, artistname_date] = parse_until(name_artistname_date, ',');
        if (name.empty())
            return LoadingResult::IncompleteLine;
        if (find_album(database, name))
            return LoadingResult::DuplicateAlbum;
        const auto [artistname, date] = parse_until(artistname_date, ',');
        if (artistname.empty() || date.empty())
            return LoadingResult::IncompleteLine;
        if (!find_artist(database, artistname))
            return LoadingResult::UnknownArtist;
        Album album{};
        album.name = name;
        album.artist_name = artistname;
        album.date = date;
        database.albums.push_back(album);
        return LoadingResult::Ok;
    }

    LoadingResult parse_and_load_song(std::string name_albumname_artistname_duration, Database& database)
    {
        const auto [name, albumname_artistname_duration] = parse_until(name_albumname_artistname_duration, ',');
        if (name.empty())
            return LoadingResult::IncompleteLine;
        if (find_song(database, name))
            return LoadingResult::DuplicateSong;
        const auto [albumname, artistname_duration] = parse_until(albumname_artistname_duration, ',');
        if (albumname.empty())
            return LoadingResult::IncompleteLine;
        if (!find_album(database, albumname))
            return LoadingResult::UnknownAlbum;
        const auto [artistname, duration] = parse_until(artistname_duration, ',');
        if (artistname.empty() || duration.empty())
            return LoadingResult::IncompleteLine;
        if (!find_artist(database, artistname))
            return LoadingResult::UnknownArtist;
        Song song{};
        song.name = name;
        song.album_name = albumname;
        song.artist_name = artistname;
        song.duration = duration;
        database.songs.push_back(song);
        return LoadingResult::Ok;
    }

    bool operator==(const Artist& artist1, const Artist& artist2)
    {
        return artist1.name == artist2.name && artist1.start_year == artist2.start_year &&
               artist1.rating == artist2.rating && artist1.genre == artist2.genre;
    }

    bool operator==(const Album& album1, const Album& album2)
    {
        return album1.name == album2.name && album1.artist_name == album2.artist_name && album1.date == album2.date;
    }

    bool operator==(const Song& song1, const Song& song2)
    {
        return song1.name == song2.name && song1.album_name == song2.album_name &&
               song1.artist_name == song2.artist_name && song1.duration == song2.duration;
    }

}} // namespace musify::database
