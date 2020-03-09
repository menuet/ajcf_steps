
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

    template <typename T>
    inline void display_music_entities(std::ostream& output_stream, const std::vector<T>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const auto& entity : music_entities)
        {
            output_stream << T::type_label << " #" << ++entity_index << ": " << entity << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << T::type_label << "s\n";
        output_stream << "-----------------\n";
    }

    LoadingResult parse_and_load_artist(std::string name_year_rating_genre, Database& database);

    LoadingResult parse_and_load_album(std::string name_artistname_date, Database& database);

    LoadingResult parse_and_load_song(std::string name_albumname_artistname_duration, Database& database);

    bool operator==(const Artist& artist1, const Artist& artist2);

    bool operator==(const Album& album1, const Album& album2);

    bool operator==(const Song& song1, const Song& song2);

}} // namespace musify::database
