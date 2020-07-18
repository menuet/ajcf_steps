
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
        mm::MenuOption{'Q', "Quit the application", option_quit},
        mm::MenuOption{'L', "Load the database", [&] { return option_load_database(database_file_path, database); }},
        mm::MenuOption{'D', "Display the database", [&] { return option_display_database(database); }},
        mm::MenuOption{'A', "Find the Artist 'Oasis'", [&] { return option_find_artist(database, "Oasis"); }},
        mm::MenuOption{'B', "Find the alBum 'Parachutes'", [&] { return option_find_album(database, "Parachutes"); }},
        mm::MenuOption{'S', "Find the Song 'Daylight'", [&] { return option_find_song(database, "Daylight"); }},
        mm::MenuOption{'E', "Find anything which name Equals 'Supreme NTM'",
                       [&] { return option_find_anything_which_name_equals(database, "Supreme NTM"); }}};

    mm::menu_loop(menu);
}
