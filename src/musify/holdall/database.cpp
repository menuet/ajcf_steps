
#include "database.hpp"
#include "database.impl.hpp"
#include "input_output.hpp"
#include <cassert>
#include <fstream>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines()
    {
        return ask_new_database_lines(std::cout, std::cin);
    }

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines)
    {
        std::ofstream database_file{database_file_path.string(), std::ios::app};
        if (!database_file)
            throw std::runtime_error{"Error: could not create or open the file"};

        for (const auto& database_line : database_lines)
            database_file << database_line << '\n';
    }

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database)
    {
        if (!std::filesystem::is_regular_file(database_file_path))
            return LoadingResult::FileNotFound;
        std::ifstream ifs{database_file_path.string()};
        if (ifs.fail())
            return LoadingResult::FileNotReadable;
        std::string database_line;
        while (std::getline(ifs, database_line))
        {
            const LoadingResult loading_result = parse_and_load_database_line(database_line, database);
            if (loading_result != LoadingResult::Ok)
                return loading_result;
        }
        return LoadingResult::Ok;
    }

    void display_database(const Database& database)
    {
        display_music_entities(std::cout, database.artists);
        display_music_entities(std::cout, database.albums);
        display_music_entities(std::cout, database.songs);
    }

    bool contains_artist(const Database& database, const std::string& artist_name)
    {
        return find_artist(database, artist_name) != nullptr;
    }

    const Artist* find_artist(const Database& database, const std::string& artist_name)
    {
        const auto iter = database.artists.find(artist_name);
        if (iter == database.artists.end())
            return nullptr;
        const auto& artist = iter->second;
        return &artist;
    }

    const Album* find_album(const Database& database, const std::string& album_name)
    {
        const auto iter = database.albums.find(album_name);
        if (iter == database.albums.end())
            return nullptr;
        const auto& albums = iter->second;
        return &albums;
    }

    const Song* find_song(const Database& database, const std::string& song_name)
    {
        for (const Song& song : database.songs)
        {
            if (song.name == song_name)
                return &song;
        }
        return nullptr;
    }

    std::ostream& operator<<(std::ostream& output_stream, const Artist& artist)
    {
        output_stream << "{" << artist.name << ", " << artist.start_year << ", " << artist.rating << ", "
                      << artist.genre << ", " << artist.albums.size() << " albums"
                      << "}";
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, const Album& album)
    {
        assert(album.artist);
        output_stream << "{" << album.name << ", " << album.artist->name << ", " << album.date << "}";
        return output_stream;
    }

    std::ostream& operator<<(std::ostream& output_stream, const Song& song)
    {
        assert(song.artist);
        assert(song.album);
        output_stream << "{" << song.name << ", " << song.album->name << ", " << song.artist->name << ", "
                      << song.duration << "}";
        return output_stream;
    }

}} // namespace musify::database
