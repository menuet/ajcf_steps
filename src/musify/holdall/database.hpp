
#pragma once

#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <string>
#include <utility>
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

    struct Database;

    enum class LoadingResult
    {
        Ok = 0,
        FileNotFound,
        FileNotReadable,
        UnknownLineType,
        IncompleteLine,
        DuplicateArtist,
        UnknownArtist,
        DuplicateAlbum,
        UnknownAlbum,
        DuplicateSong,
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database** database);

    void release_database(Database* database);

    void display_database(const Database* database);

    const Artist* find_artist(const Database* database, const std::string& artist_name);

    const Album* find_album(const Database* database, const std::string& album_name);

    const Song* find_song(const Database* database, const std::string& song_name);

    std::ostream& operator<<(std::ostream& output_stream, const Artist& artist);

    std::ostream& operator<<(std::ostream& output_stream, const Album& album);

    std::ostream& operator<<(std::ostream& output_stream, const Song& song);

}} // namespace musify::database
