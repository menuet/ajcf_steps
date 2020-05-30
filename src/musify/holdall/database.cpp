
#include "database.hpp"
#include "database.impl.hpp"
#include "input_output.hpp"
#include <algorithm>
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

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database** database)
    {
        *database = nullptr;
        if (!std::filesystem::is_regular_file(database_file_path))
            return LoadingResult::FileNotFound;
        std::ifstream ifs{database_file_path.string()};
        if (ifs.fail())
            return LoadingResult::FileNotReadable;
        std::string database_line;
        auto db = std::make_unique<Database>();
        while (std::getline(ifs, database_line))
        {
            const LoadingResult loading_result = parse_and_load_database_line(database_line, *db);
            if (loading_result != LoadingResult::Ok)
                return loading_result;
        }
        *database = db.release();
        return LoadingResult::Ok;
    }

    void release_database(Database* database)
    {
        delete database;
    }

    void display_database(const Database* database)
    {
        assert(database);
        display_music_entities(std::cout, database->artists);
        display_music_entities(std::cout, database->albums);
        display_music_entities(std::cout, database->songs);
    }

    const Artist* find_artist(const Database* database, const std::string& artist_name)
    {
        assert(database);
        const auto iter = database->artists.find(artist_name);
        if (iter == database->artists.end())
            return nullptr;
        const auto& artist = iter->second;
        return &artist;
    }

    const Album* find_album(const Database* database, const std::string& album_name)
    {
        assert(database);
        const auto iter = database->albums.find(album_name);
        if (iter == database->albums.end())
            return nullptr;
        const auto& albums = iter->second;
        return &albums;
    }

    const Song* find_song(const Database* database, const std::string& song_name)
    {
        assert(database);
        const auto iter = std::find_if(database->songs.begin(), database->songs.end(),
                                       [&](const auto& song) { return song.name == song_name; });
        if (iter == database->songs.end())
            return nullptr;
        return &*iter;
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
