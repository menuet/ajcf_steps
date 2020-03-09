
#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace musify { namespace database {

    enum class LoadingResult;
    struct Database;
    struct Artist;
    struct Album;
    struct Song;

    std::vector<std::string> ask_new_database_lines(std::ostream& output_stream, std::istream& input_stream);

    std::pair<std::string, std::string> parse_until(const std::string& text, char separator);

    LoadingResult parse_and_load_database_line(const std::string& line, Database& database);

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Artist>& music_entities);

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Album>& music_entities);

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<Song>& music_entities);

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<std::string>& music_entities);

    LoadingResult parse_and_load_artist(std::string name_year_rating_genre, Database& database);

    LoadingResult parse_and_load_album(std::string name_artistname_date, Database& database);

    LoadingResult parse_and_load_song(std::string name_albumname_artistname_duration, Database& database);

}} // namespace musify::database
