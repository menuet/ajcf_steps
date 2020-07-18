
#pragma once

#include "musical_base.hpp"
#include "musical_factory.hpp"
#include "strong_types.hpp"
#include <string_view>
#include <vector>

namespace musify::database {

    class Artist final : public MusicalBase
    {
        friend class Database;

    public:
        static constexpr std::string_view type_label{"Artist"};

        static MusicalThingRegistrar<Artist> registrar;

        friend bool operator==(const Artist& left, const Artist& right)
        {
            return left.name() == right.name() && left.m_genre == right.m_genre;
        }

        friend bool operator!=(const Artist& left, const Artist& right)
        {
            return !(left == right);
        }

        Artist(std::string name) : MusicalBase{name}
        {
        }

        Artist(std::string name, strong::Year start_year, strong::Rating rating, strong::Genre genre)
            : MusicalBase{name}, m_start_year{start_year}, m_rating{rating}, m_genre{genre}
        {
        }

        virtual std::string_view concrete_type_label() const final
        {
            return type_label;
        }

        virtual ParsingResult parse_details(std::string_view details) final;

    private:
        virtual void details_to_stream(std::ostream& output_stream) const final;

        strong::Year m_start_year{};
        strong::Rating m_rating{};
        strong::Genre m_genre{};
    };

    class Album final : public MusicalBase
    {
    public:
        static constexpr std::string_view type_label{"Album"};

        static MusicalThingRegistrar<Album> registrar;

        friend bool operator==(const Album& left, const Album& right)
        {
            return left.name() == right.name();
        }

        friend bool operator!=(const Album& left, const Album& right)
        {
            return !(left == right);
        }

        Album(std::string name) : MusicalBase{name}
        {
        }

        Album(std::string name, std::string artist_name, strong::Date date)
            : MusicalBase{name}, m_artist_name{artist_name}, m_date{date}
        {
        }

        virtual std::string_view concrete_type_label() const final
        {
            return type_label;
        }

        virtual ParsingResult parse_details(std::string_view details) final;

    private:
        virtual void details_to_stream(std::ostream& output_stream) const final;

        std::string m_artist_name{};
        strong::Date m_date{};
    };

    class Song final : public MusicalBase
    {
    public:
        static constexpr std::string_view type_label{"Song"};

        static MusicalThingRegistrar<Song> registrar;

        friend bool operator==(const Song& left, const Song& right)
        {
            return left.name() == right.name();
        }

        friend bool operator!=(const Song& left, const Song& right)
        {
            return !(left == right);
        }

        Song(std::string name) : MusicalBase{name}
        {
        }

        Song(std::string name, std::string album_name, std::string artist_name, strong::Duration duration)
            : MusicalBase{name}, m_album_name{album_name}, m_artist_name{artist_name}, m_duration{duration}
        {
        }

        virtual std::string_view concrete_type_label() const final
        {
            return type_label;
        }

        virtual ParsingResult parse_details(std::string_view details) final;

    private:
        virtual void details_to_stream(std::ostream& output_stream) const final;

        std::string m_album_name{};
        std::string m_artist_name{};
        strong::Duration m_duration{};
    };

} // namespace musify::database
