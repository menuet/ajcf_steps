
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
        std::vector<std::string> lines{"Artist=1", "Album=2", "Song=3", "# Comment", "Song=4", "Artist=5", "Song=6"};
        save_new_database_lines(database_file_path, lines);

        // ACT
        Database database;
        const auto result = load_database(database_file_path, database);

        // ASSERT
        REQUIRE(result == LoadingResult::Ok);
        REQUIRE(database.artists == std::vector<std::string>{"1", "5"});
        REQUIRE(database.albums == std::vector<std::string>{"2"});
        REQUIRE(database.songs == std::vector<std::string>{"3", "4", "6"});
    }

    TEST_CASE("TEST musify::database::display_music_entities", "[database]")
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

}} // namespace musify::database
