
#pragma once

#include "musical_things.hpp"
#include "singleton.hpp"
#include <string_view>
#include <unordered_map>
#include <algorithm>
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

    enum class LoadingResult
    {
        Ok = 0,
        FileNotFound = 1,
        FileNotReadable = 2,
        UnknownLineType = 3,
        IncompleteLine = 4,
        ParsingError = 5,
        DuplicateArtist = 6,
        UnknownArtist,
        DuplicateAlbum,
        UnknownAlbum,
        DuplicateSong,
    };

    enum class InsertionResult
    {
        Ok = 0,
        DuplicateArtist = 6,
        UnknownArtist,
        DuplicateAlbum,
        UnknownAlbum,
        DuplicateSong,
    };

    class Database
    {
        friend class singleton::Singleton<Database>;

        Database() = default;

    public:
        using Artists = BintreeOrHashtable<std::string, Artist>;
        using Albums = BintreeOrHashtable<std::string, Album>;
        using Songs = std::list<Song>;
        using ConstMusicalThingRef = std::reference_wrapper<MusicalThing const>;
        using MusicalThings = std::vector<ConstMusicalThingRef>;

        Database(const Database&) = delete;

        Database& operator=(const Database&) = delete;

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

        template <typename ThingVisitorT>
        void visit_things(ThingVisitorT visitor) const
        {
            std::for_each(m_things.begin(), m_things.end(),
                          [&](const auto& name_and_thing) { visitor(*name_and_thing.second); });
        }

        void clear()
        {
            m_artists.clear();
            m_albums.clear();
            m_songs.clear();
            m_things.clear();
        }

        InsertionResult insert_artist(std::string name, strong::Year start_year, strong::Rating rating,
                                      strong::Genre genre);

        InsertionResult insert_album(std::string name, std::string artist_name, strong::Date);

        InsertionResult insert_song(std::string name, std::string album_name, std::string artist_name,
                                    strong::Duration duration);

    private:
        Artists m_artists{};
        Albums m_albums{};
        Songs m_songs{};
        std::multimap<std::string, const MusicalThing*> m_things{};
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database);

}} // namespace musify::database
