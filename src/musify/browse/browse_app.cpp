
#include "browse_app.hpp"
#include "menu_options.hpp"
#include <optional>

std::optional<fs::path> check_arguments(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Please, provide the database file path as an argument\n";
        return std::nullopt;
    }

    fs::path database_file_path{argv[1]};
    if (database_file_path.is_relative())
        database_file_path = fs::current_path() / database_file_path;

    return database_file_path;
}

void menu_loop(const fs::path& database_file_path)
{
    mdb::Database database;
    mm::Menu menu{
        mm::MenuOption{'Q', "Quit the application", std::make_shared<OptionQuit>()},
        mm::MenuOption{'L', "Load the database", std::make_shared<OptionLoadDatabase>(database_file_path, database)},
        mm::MenuOption{'D', "Display the database", std::make_shared<OptionDisplayDatabase>(database)},
        mm::MenuOption{'A', "Find the Artist 'Oasis'", std::make_shared<OptionFindArtist>(database, "Oasis")},
        mm::MenuOption{'B', "Find the alBum 'Parachutes'", std::make_shared<OptionFindAlbum>(database, "Parachutes")},
        mm::MenuOption{'S', "Find the Song 'Daylight'", std::make_shared<OptionFindSong>(database, "Daylight")},
        mm::MenuOption{'E', "Find anything which name Equals 'Supreme NTM'",
                       std::make_shared<OptionFindAnythingWhichNameEquals>(database, "Supreme NTM")}};

    mm::menu_loop(menu);
}
