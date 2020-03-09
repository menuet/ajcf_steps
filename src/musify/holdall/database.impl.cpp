
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

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Artist>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const auto& artist : music_entities)
        {
            output_stream << entity_type << " #" << ++entity_index << ": "
                          << "{" << artist.name << ", " << artist.start_year << ", " << artist.rating << ", "
                          << artist.genre << "}"
                          << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << entity_type << "s\n";
        output_stream << "-----------------\n";
    }

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Album>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const auto& album : music_entities)
        {
            output_stream << entity_type << " #" << ++entity_index << ": "
                          << "{" << album.name << ", " << album.artist_name << ", " << album.date << "}"
                          << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << entity_type << "s\n";
        output_stream << "-----------------\n";
    }

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Song>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const auto& song : music_entities)
        {
            output_stream << entity_type << " #" << ++entity_index << ": "
                          << "{" << song.name << ", " << song.album_name << ", " << song.artist_name << ", "
                          << song.duration << "}"
                          << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << entity_type << "s\n";
        output_stream << "-----------------\n";
    }

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<std::string>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const std::string& music_entity : music_entities)
        {
            output_stream << entity_type << " #" << ++entity_index << ": " << music_entity << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << entity_type << "s\n";
        output_stream << "-----------------\n";
    }

    LoadingResult parse_and_load_artist(std::string name_year_rating_genre, Database& database)
    {
        const auto [name, year_rating_genre] = parse_until(name_year_rating_genre, ',');
        if (name.empty())
            return LoadingResult::IncompleteLine;
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
        const auto [artistname, date] = parse_until(artistname_date, ',');
        if (artistname.empty() || date.empty())
            return LoadingResult::IncompleteLine;
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
        const auto [albumname, artistname_duration] = parse_until(albumname_artistname_duration, ',');
        if (albumname.empty())
            return LoadingResult::IncompleteLine;
        const auto [artistname, duration] = parse_until(artistname_duration, ',');
        if (artistname.empty() || duration.empty())
            return LoadingResult::IncompleteLine;
        Song song{};
        song.name = name;
        song.album_name = albumname;
        song.artist_name = artistname;
        song.duration = duration;
        database.songs.push_back(song);
        return LoadingResult::Ok;
    }

}} // namespace musify::database
