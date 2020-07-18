
#pragma once

#include "database.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace musify { namespace database {

    enum class LoadingResult;
    class Database;
    class Artist;
    class Album;
    class Song;

    std::vector<std::string> ask_new_database_lines(std::ostream& output_stream, std::istream& input_stream);

    std::pair<std::string, std::string> parse_until(const std::string& text, char separator);

    LoadingResult parse_and_load_database_line(const std::string& line, Database& database);

    template <typename NameT, typename EntityT>
    const auto& get_entity_only(const std::pair<const NameT, EntityT>& name_and_entity)
    {
        return name_and_entity.second;
    }

    template <typename EntityT>
    const auto& get_entity_only(const EntityT& entity)
    {
        return entity;
    }

    template <typename ContainerT>
    struct EntityOnly;

    template <typename ContainerT>
    using EntityOnly_t = typename EntityOnly<ContainerT>::type;

    template <typename KeyT, typename EntityT>
    struct EntityOnly<BintreeOrHashtable<KeyT, EntityT>>
    {
        using type = EntityT;
    };

    template <typename EntityT>
    struct EntityOnly<std::vector<EntityT>>
    {
        using type = EntityT;
    };

    template <typename EntityT>
    struct EntityOnly<std::list<EntityT>>
    {
        using type = EntityT;
    };

    template <typename ContainerT>
    inline void display_music_entities(std::ostream& output_stream, const ContainerT& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const auto& name_and_entity : music_entities)
        {
            output_stream << EntityOnly_t<ContainerT>::type_label << " #" << ++entity_index << ": "
                          << get_entity_only(name_and_entity) << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << EntityOnly_t<ContainerT>::type_label << "s\n";
        output_stream << "-----------------\n";
    }

    LoadingResult parse_and_load_artist(std::string name_year_rating_genre, Database& database);

    LoadingResult parse_and_load_album(std::string name_artistname_date, Database& database);

    LoadingResult parse_and_load_song(std::string name_albumname_artistname_duration, Database& database);

    bool operator==(const Artist& artist1, const Artist& artist2);

    bool operator==(const Album& album1, const Album& album2);

    bool operator==(const Song& song1, const Song& song2);

}} // namespace musify::database
