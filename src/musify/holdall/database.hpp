
#pragma once

#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <list>
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

    class Database;
    class Album;

    class MusicalThing
    {
    public:
        friend std::ostream& operator<<(std::ostream& output_stream, const MusicalThing& thing);

        MusicalThing(std::string name, std::string_view type_label) : m_name{name}, m_type_label{type_label}
        {
        }

        const std::string& name() const
        {
            return m_name;
        }

        const std::string_view& type_label() const
        {
            return m_type_label;
        }

    private:
        std::string m_name{};
        std::string_view m_type_label{};
    };

    class Artist : public MusicalThing
    {
        friend class Database;

    public:
        using Albums = std::vector<const Album*>;

        static constexpr std::string_view type_label{"Artist"};

        friend std::ostream& operator<<(std::ostream& output_stream, const Artist& artist);

        friend bool operator==(const Artist& left, const Artist& right)
        {
            return left.name() == right.name() && left.m_genre == right.m_genre;
        }

        friend bool operator!=(const Artist& left, const Artist& right)
        {
            return !(left == right);
        }

        Artist(std::string name, std::string start_year, std::string rating, std::string genre)
            : MusicalThing{name, type_label}, m_start_year{start_year}, m_rating{rating}, m_genre{genre}
        {
        }

        const Albums& albums() const
        {
            return m_albums;
        }

    private:
        std::string m_start_year{};
        std::string m_rating{};
        std::string m_genre{};
        std::vector<const Album*> m_albums{};
    };

    class Album : public MusicalThing
    {
    public:
        static constexpr std::string_view type_label{"Album"};

        friend std::ostream& operator<<(std::ostream& output_stream, const Album& album);

        friend bool operator==(const Album& left, const Album& right)
        {
            return left.name() == right.name() && left.m_artist == right.m_artist;
        }

        friend bool operator!=(const Album& left, const Album& right)
        {
            return !(left == right);
        }

        Album(std::string name, const Artist* artist, std::string date)
            : MusicalThing{name, type_label}, m_artist{artist}, m_date{date}
        {
        }

    private:
        const Artist* m_artist{};
        std::string m_date{};
    };

    class Song : public MusicalThing
    {
    public:
        static constexpr std::string_view type_label{"Song"};

        friend std::ostream& operator<<(std::ostream& output_stream, const Song& song);

        friend bool operator==(const Song& left, const Song& right)
        {
            return left.name() == right.name() && left.m_album == right.m_album && left.m_artist == right.m_artist;
        }

        friend bool operator!=(const Song& left, const Song& right)
        {
            return !(left == right);
        }

        Song(std::string name, const Album* album, const Artist* artist, std::string duration)
            : MusicalThing{name, type_label}, m_album{album}, m_artist{artist}, m_duration{duration}
        {
        }

    private:
        const Album* m_album{};
        const Artist* m_artist{};
        std::string m_duration{};
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
        using Songs = std::list<Song>;
        using ConstMusicalThingRef = std::reference_wrapper<MusicalThing const>;
        using MusicalThings = std::vector<ConstMusicalThingRef>;

        void display() const;

        const Artist* find_artist(const std::string& artist_name) const;

        const Album* find_album(const std::string& album_name) const;

        const Song* find_song(const std::string& song_name) const;

        MusicalThings find_things(const std::string& name) const;

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
        Artists m_artists{};
        Albums m_albums{};
        Songs m_songs{};
        std::multimap<std::string, const MusicalThing*> m_things{};
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database);

}} // namespace musify::database
