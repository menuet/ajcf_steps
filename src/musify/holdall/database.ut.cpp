
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
        REQUIRE(database.artists().size() == 1);
        REQUIRE(database.artists().begin()->second == Artist{"Artist1", "2001", "4.5", "Rock"});
        REQUIRE(database.albums().size() == 1);
        REQUIRE(database.albums().begin()->second ==
                Album{"Album1", &database.artists().begin()->second, "2020/03/09"});
        REQUIRE(database.songs() == std::list<Song>{{"Song1", &database.albums().begin()->second,
                                                     &database.artists().begin()->second, "3:45"}});
    }

    TEST_CASE("TEST musify::database::display_music_entities with artists", "[database]")
    {
        // ARRANGE
        std::stringstream output_stream;
        std::vector<Artist> music_artists{
            {"a1", "2000", "1.5", "Pop"}, {"a2", "2001", "3.", "Rock"}, {"a3", "2002", "5.0", "Jazz"}};

        // ACT
        display_music_entities(output_stream, music_artists);

        // ASSERT
        REQUIRE(output_stream.str() ==
                "-----------------\n"
                "Artist #1: {a1, 2000, 1.5, Pop, 0 albums}\n"
                "-----------------\n"
                "Artist #2: {a2, 2001, 3., Rock, 0 albums}\n"
                "-----------------\n"
                "Artist #3: {a3, 2002, 5.0, Jazz, 0 albums}\n"
                "-----------------\n"
                "--> 3 Artists\n"
                "-----------------\n");
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
        // ARRANGE
        Database database{};
        database.insert_artist("Oasis", "1991", "3.7", "Pop");

        // ACT
        const auto result = parse_and_load_album("Morning Glory,Oasis,1995/10/02", database);

        // ASSERT
        REQUIRE(result == LoadingResult::Ok);
        REQUIRE(database.albums().size() == 1);
        REQUIRE(database.albums().begin()->second ==
                Album{"Morning Glory", &database.artists().begin()->second, "1995/10/02"});
    } // namespace database

    TEST_CASE("TEST musify::database::parse_and_load_song", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::Database::find_things", "[database]")
    {
        // ARRANGE
        Database database{};
        database.insert_artist("U2", "1976", "4.5", "Rock");
        database.insert_album("War", "U2", "1983/03/21");
        database.insert_song("Sunday Bloody Sunday", "War", "U2", "4:40");
        // Insert more dummy songs to be confident that we fixed the "realloc bug"
        database.insert_song("Dummy 1", "War", "U2", "4:40");
        database.insert_song("Dummy 2", "War", "U2", "4:40");
        database.insert_song("Dummy 3", "War", "U2", "4:40");
        database.insert_song("Dummy 4", "War", "U2", "4:40");

        // ACT
        const auto things = database.find_things("Sunday Bloody Sunday");

        // ASSERT
        REQUIRE(things.size() == 1);
        REQUIRE(things[0].get().type_label() == "Song");
    }

}} // namespace musify::database
