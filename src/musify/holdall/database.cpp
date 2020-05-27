
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

    static std::size_t compute_max_number_of_entities(std::ifstream& ifs)
    {
        std::string database_line;
        std::size_t count{0};
        while (ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n'))
        {
            ++count;
        }
        ifs.clear();
        ifs.seekg(0, std::ios_base::beg);
        return count;
    }

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database)
    {
        if (!std::filesystem::is_regular_file(database_file_path))
            return LoadingResult::FileNotFound;
        std::ifstream ifs{database_file_path.string()};
        if (ifs.fail())
            return LoadingResult::FileNotReadable;
        const auto max_entities_count = compute_max_number_of_entities(ifs);
        database.artists.reserve(max_entities_count);
        database.albums.reserve(max_entities_count);
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
        for (const Artist& artist : database.artists)
        {
            if (artist.name == artist_name)
                return &artist;
        }
        return nullptr;
    }

    const Album* find_album(const Database& database, const std::string& album_name)
    {
        for (const Album& album : database.albums)
        {
            if (album.name == album_name)
                return &album;
        }
        return nullptr;
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
                      << artist.genre << "}";
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
