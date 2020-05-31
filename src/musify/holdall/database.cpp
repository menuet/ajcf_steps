
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

    void Database::display() const
    {
        display_music_entities(std::cout, this->m_artists);
        display_music_entities(std::cout, this->m_albums);
        display_music_entities(std::cout, this->m_songs);
    }

    const Artist* Database::find_artist(const std::string& artist_name) const
    {
        const auto iter = this->m_artists.find(artist_name);
        if (iter == this->m_artists.end())
            return nullptr;
        const auto& artist = iter->second;
        return &artist;
    }

    const Album* Database::find_album(const std::string& album_name) const
    {
        const auto iter = this->m_albums.find(album_name);
        if (iter == this->m_albums.end())
            return nullptr;
        const auto& albums = iter->second;
        return &albums;
    }

    const Song* Database::find_song(const std::string& song_name) const
    {
        for (const Song& song : this->m_songs)
        {
            if (song.name == song_name)
                return &song;
        }
        return nullptr;
    }

    InsertionResult Database::insert_artist(std::string name, std::string start_year, std::string rating,
                                            std::string genre)
    {
        assert(!name.empty());

        if (this->find_artist(name))
            return InsertionResult::DuplicateArtist;
        this->m_artists.insert({name, {name, start_year, rating, genre, {}}});
        return InsertionResult::Ok;
    }

    InsertionResult Database::insert_album(std::string name, std::string artist_name, std::string date)
    {
        assert(!name.empty());

        if (find_album(name))
            return InsertionResult::DuplicateAlbum;
        const auto artist = find_artist(artist_name);
        if (!artist)
            return InsertionResult::UnknownArtist;
        const auto iter_and_result = this->m_albums.insert({name, {name, artist, date}});
        const auto& album = iter_and_result.first->second;
        Artist* mutable_artist = const_cast<Artist*>(artist);
        mutable_artist->albums.push_back(&album);
        return InsertionResult::Ok;
    }

    InsertionResult Database::insert_song(std::string name, std::string album_name, std::string artist_name,
                                          std::string duration)
    {
        assert(!name.empty());

        if (find_song(name))
            return InsertionResult::DuplicateSong;
        const auto album = find_album(album_name);
        if (!album)
            return InsertionResult::UnknownAlbum;
        const auto artist = find_artist(artist_name);
        if (!artist)
            return InsertionResult::UnknownArtist;
        Song song{};
        song.name = name;
        song.album = album;
        song.artist = artist;
        song.duration = duration;
        this->m_songs.push_back(song);
        return InsertionResult::Ok;
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
