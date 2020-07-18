
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

static void display_menu()
{
    std::cout << "Please select an option\n";
    std::cout << "Q -> Quit the application\n";
    std::cout << "L -> Load the database\n";
    std::cout << "D -> Display the database\n";
    std::cout << "A -> Find the Artist 'Oasis'\n";
    std::cout << "B -> Find the alBum 'Parachutes'\n";
    std::cout << "S -> Find the Song 'Daylight'\n";
    std::cout << "E -> Find anything which name Equals 'Supreme NTM'\n";
}

static char ask_user_choice()
{
    char user_choice{};
    std::cin >> user_choice;
    return user_choice;
}

void menu_loop(const fs::path& database_file_path)
{
    mdb::Database database;
    for (;;)
    {
        display_menu();
        const auto user_choice = ask_user_choice();
        switch (user_choice)
        {
        case 'Q':
            option_quit();
            return;
        case 'L':
            option_load_database(database_file_path, database);
            break;
        case 'D':
            option_display_database(database);
            break;
        case 'A':
            option_find_artist(database, "Oasis");
            break;
        case 'B':
            option_find_album(database, "Parachutes");
            break;
        case 'S':
            option_find_song(database, "Daylight");
            break;
        case 'E':
            option_find_anything_which_name_equals(database, "Supreme NTM");
            break;
        default:
            std::cout << "Please enter a valid choice\n";
            break;
        }
    }
}
