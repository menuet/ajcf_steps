
#include "database.hpp"
#include "database.impl.hpp"
#include "input_output.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>

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
            if (song.name() == song_name)
                return &song;
        }
        return nullptr;
    }

    Database::MusicalThings Database::find_things(const std::string& name) const
    {
        const auto [iter_begin, iter_end] = m_things.equal_range(name);
        MusicalThings things{};
        std::transform(iter_begin, iter_end, std::back_inserter(things),
                       [](const auto& name_and_thing) { return std::cref(*name_and_thing.second); });
        return things;
    }

    void Database::visit_things(ThingVisitor& visitor) const
    {
        std::for_each(m_things.begin(), m_things.end(),
                      [&](const auto& name_and_thing) { visitor.visit(*name_and_thing.second); });
    }

    InsertionResult Database::insert_artist(std::string name, strong::Year start_year, strong::Rating rating,
                                            strong::Genre genre)
    {
        assert(!name.empty());

        const auto [iter_artist, result] = m_artists.emplace(name, Artist{name, start_year, rating, genre});
        if (!result)
            return InsertionResult::DuplicateArtist;
        const Artist& artist = iter_artist->second;

        m_things.emplace(name, &artist);

        return InsertionResult::Ok;
    }

    InsertionResult Database::insert_album(std::string name, std::string artist_name, strong::Date date)
    {
        assert(!name.empty());

        const auto iter_artist = m_artists.find(artist_name);
        if (iter_artist == m_artists.end())
            return InsertionResult::UnknownArtist;
        Artist& artist = iter_artist->second;

        const auto [iter_album, result] = m_albums.emplace(name, Album{name, &artist, date});
        if (!result)
            return InsertionResult::DuplicateAlbum;
        const Album& album = iter_album->second;

        artist.m_albums.push_back(&album);

        m_things.emplace(name, &album);

        return InsertionResult::Ok;
    }

    InsertionResult Database::insert_song(std::string name, std::string album_name, std::string artist_name,
                                          strong::Duration duration)
    {
        assert(!name.empty());

        const auto iter_album = m_albums.find(album_name);
        if (iter_album == m_albums.end())
            return InsertionResult::UnknownAlbum;
        const Album& album = iter_album->second;

        const auto iter_artist = m_artists.find(artist_name);
        if (iter_artist == m_artists.end())
            return InsertionResult::UnknownArtist;
        const Artist& artist = iter_artist->second;

        const auto iter_song =
            std::find_if(m_songs.begin(), m_songs.end(), [&](const auto& song) { return song.name() == name; });
        if (iter_song != m_songs.end())
            return InsertionResult::DuplicateSong;
        m_songs.emplace_back(name, &album, &artist, duration);
        const Song& song = m_songs.back();

        m_things.emplace(name, &song);

        return InsertionResult::Ok;
    }

    std::ostream& operator<<(std::ostream& output_stream, const MusicalThing& thing)
    {
        thing.to_stream(output_stream);
        return output_stream;
    }

    void MusicalBase::to_stream(std::ostream& output_stream) const
    {
        output_stream << "{" << name() << ", " << concrete_type_label() << "}";
    }

    void Artist::to_stream(std::ostream& output_stream) const
    {
        output_stream << "{";
        MusicalBase::to_stream(output_stream);
        output_stream << ", " << m_start_year << ", " << m_rating << ", " << m_genre << ", " << m_albums.size()
                      << " albums"
                      << "}";
    }

    void Album::to_stream(std::ostream& output_stream) const
    {
        output_stream << "{";
        MusicalBase::to_stream(output_stream);
        output_stream << ", " << m_artist->name() << ", " << m_date << "}";
    }

    void Song::to_stream(std::ostream& output_stream) const
    {
        output_stream << "{";
        MusicalBase::to_stream(output_stream);
        output_stream << ", " << m_album->name() << ", " << m_artist->name() << ", " << m_duration << "}";
    }

}} // namespace musify::database
