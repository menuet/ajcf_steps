
#pragma once

#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#if defined(TRY_RISKY_FIX)
template <typename KeyT, typename DataT>
using BintreeOrHashtable = std::unordered_map<KeyT, DataT>;
#else
template <typename KeyT, typename DataT>
using BintreeOrHashtable = std::map<KeyT, DataT>;
#endif

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines();

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines);

    struct Album;

    struct Artist
    {
        static constexpr std::string_view type_label{"Artist"};

        std::string name;
        std::string start_year;
        std::string rating;
        std::string genre;
        std::vector<const Album*> albums;
    };

    struct Album
    {
        static constexpr std::string_view type_label{"Album"};

        std::string name;
        const Artist* artist;
        std::string date;
    };

    struct Song
    {
        static constexpr std::string_view type_label{"Song"};

        std::string name;
        const Album* album;
        const Artist* artist;
        std::string duration;
    };

    enum class LoadingResult
    {
        Ok = 0,
        FileNotFound = 1,
        FileNotReadable = 2,
        UnknownLineType = 3,
        IncompleteLine = 4,
        DuplicateArtist = 5,
        UnknownArtist,
        DuplicateAlbum,
        UnknownAlbum,
        DuplicateSong,
    };

    enum class InsertionResult
    {
        Ok = 0,
        DuplicateArtist = 5,
        UnknownArtist,
        DuplicateAlbum,
        UnknownAlbum,
        DuplicateSong,
    };

    class Database
    {
    public:
        using Artists = BintreeOrHashtable<std::string, Artist>;
        using Albums = BintreeOrHashtable<std::string, Album>;
        using Songs = std::vector<Song>;

        void display() const;

        const Artist* find_artist(const std::string& artist_name) const;

        const Album* find_album(const std::string& album_name) const;

        const Song* find_song(const std::string& song_name) const;

        const Artists& artists() const
        {
            return m_artists;
        }

        const Albums& albums() const
        {
            return m_albums;
        }

        const Songs& songs() const
        {
            return m_songs;
        }

        InsertionResult insert_artist(std::string name, std::string start_year, std::string rating, std::string genre);

        InsertionResult insert_album(std::string name, std::string artist_name, std::string date);

        InsertionResult insert_song(std::string name, std::string album_name, std::string artist_name,
                                    std::string duration);

    private:
        BintreeOrHashtable<std::string, Artist> m_artists{};
        BintreeOrHashtable<std::string, Album> m_albums{};
        std::vector<Song> m_songs{};
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database);

    std::ostream& operator<<(std::ostream& output_stream, const Artist& artist);

    std::ostream& operator<<(std::ostream& output_stream, const Album& album);

    std::ostream& operator<<(std::ostream& output_stream, const Song& song);

}} // namespace musify::database
