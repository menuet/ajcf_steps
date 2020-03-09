
#include "database.hpp"
#include "database.impl.hpp"
#include <catch2/catch.hpp>
#include <sstream>

namespace musify { namespace database {

    TEST_CASE("TEST musify::database::ask_new_database_lines WITH a few lines", "[database]")
    {
        // ARRANGE
        std::stringstream input_stream{
            "y\n"
            "Line 1\n"
            "Y\n"
            "Line 2\n"
            "y\n"
            "Line 3\n"
            "n\n"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_new_database_lines(output_stream, input_stream);

        // ASSERT
        REQUIRE(result == std::vector<std::string>{"Line 1", "Line 2", "Line 3"});
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() ==
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? ");
    }

    TEST_CASE("TEST musify::database::save_new_database_lines WITH a few lines", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Line 1", "Line 2", "Line 3", "Line 4"};

        // ACT
        save_new_database_lines(database_file_path, lines);

        // ASSERT
        REQUIRE(fs::is_regular_file(database_file_path));
        REQUIRE(fs::file_size(database_file_path) > 0);
    }

    TEST_CASE("TEST musify::database::load_database WITH file containing a few entities of unknown type", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Line 1", "Line 2", "Line 3", "Line 4"};
        save_new_database_lines(database_file_path, lines);

        // ACT
        Database database;
        const auto result = load_database(database_file_path, database);

        // ASSERT
        REQUIRE(result == LoadingResult::UnknownLineType);
    }

    TEST_CASE(
        "TEST musify::database::load_database WITH file containing a few entities, some of them of incomplete type",
        "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Artist=1", "Album=2", "Song=3", "Song="};
        save_new_database_lines(database_file_path, lines);

        // ACT
        Database database;
        const auto result = load_database(database_file_path, database);

        // ASSERT
        REQUIRE(result == LoadingResult::IncompleteLine);
    }

    TEST_CASE("TEST musify::database::load_database WITH file containing a few entities", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Artist=Artist1,2001,4.5,Rock", "Album=Album1,Artist1,2020/03/09",
                                       "Song=Song1,Album1,Artist1,3:45"};
        save_new_database_lines(database_file_path, lines);

        // ACT
        Database database;
        const auto result = load_database(database_file_path, database);

        // ASSERT
        REQUIRE(result == LoadingResult::Ok);

        // Does not compile yet:
        // REQUIRE(database.artists == std::vector<Artist>{{"Artist1", "2001", "4.5", "Rock"}});

        REQUIRE(database.artists.size() == 1);
        const Artist& artist = database.artists[0];
        REQUIRE(artist.name == "Artist1");
        REQUIRE(artist.start_year == "2001");
        REQUIRE(artist.rating == "4.5");
        REQUIRE(artist.genre == "Rock");

        // Does not compile yet:
        // REQUIRE(database.albums == std::vector<Album>{{"Album1", "Artist1", "2020/03/09"}});

        REQUIRE(database.albums.size() == 1);
        const Album& album = database.albums[0];
        REQUIRE(album.name == "Album1");
        REQUIRE(album.artist_name == "Artist1");
        REQUIRE(album.date == "2020/03/09");

        // Does not compile yet:
        // REQUIRE(database.songs == std::vector<Song>{{"Song1", "Album1", "Artist1", "3:45"}});

        REQUIRE(database.songs.size() == 1);
        const Song& song = database.songs[0];
        REQUIRE(song.name == "Song1");
        REQUIRE(song.album_name == "Album1");
        REQUIRE(song.artist_name == "Artist1");
        REQUIRE(song.duration == "3:45");
    }

    TEST_CASE("TEST musify::database::display_music_entities with strings", "[database]")
    {
        // ARRANGE
        std::stringstream output_stream;
        std::vector<std::string> music_entities{"a1", "a2", "a3"};

        // ACT
        display_music_entities(output_stream, "Album", music_entities);

        // ASSERT
        REQUIRE(output_stream.str() ==
                "-----------------\n"
                "Album #1: a1\n"
                "-----------------\n"
                "Album #2: a2\n"
                "-----------------\n"
                "Album #3: a3\n"
                "-----------------\n"
                "--> 3 Albums\n"
                "-----------------\n");
    }

    TEST_CASE("TEST musify::database::display_music_entities with artists", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::display_music_entities with albums", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::display_music_entities with songs", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::parse_and_load_artist", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::parse_and_load_album", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::parse_and_load_song", "[database]")
    {
        // TODO
    }

}} // namespace musify::database
