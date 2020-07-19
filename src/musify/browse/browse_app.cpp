
#include "browse_app.hpp"
#include "menu_options.hpp"
#include <holdall/actor.hpp>
#include <holdall/singleton.hpp>
#include <optional>
#include <thread>

namespace ms = musify::singleton;
namespace mo = musify::observer;
namespace ma = musify::actor;

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

static void log_thread_id()
{
    std::cout << "In thread " << std::this_thread::get_id() << "\n";
}

void menu_loop(const fs::path& database_file_path)
{
    log_thread_id();

    ma::Actor actor{};
    mdb::Database& database = ms::Singleton<mdb::Database>::get_instance();
    const auto observer = mo::make_observer<mdb::Event>([](const mdb::Event& event) {
        log_thread_id();
        std::cout << "Event received: " << (event == mdb::Event::Insert ? "Insert" : "Clear") << '\n';
    });

    mm::Menu menu{mm::MenuOption{'Q', "Quit the application", option_quit},
                  mm::MenuOption{'L', "Load the database",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_load_database(database_file_path, database);
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'D', "Display the database",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_display_database(database);
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'A', "Find the Artist 'Oasis'",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_find_artist(database, "Oasis");
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'B', "Find the alBum 'Parachutes'",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_find_album(database, "Parachutes");
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'S', "Find the Song 'Daylight'",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_find_song(database, "Daylight");
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'E', "Find anything which name Equals 'Supreme NTM'",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_find_anything_which_name_equals(database, "Supreme NTM");
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'C', "Find anything which name Contains 'ol'",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();
                                         option_find_anything_which_name_contains(database, "ol");
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'X', "Attach the observer",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();

                                         std::cout << "Attaching observer\n";
                                         database.attach_observer(observer);
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'Y', "Detach the observer",
                                 [&] {
                                     actor.push_task([&] {
                                         log_thread_id();

                                         std::cout << "Detaching observer\n";
                                         database.detach_observer(observer);
                                     });
                                     return true;
                                 }},
                  mm::MenuOption{'Z', "Clear the database", [&] {
                                     actor.push_task([&] {
                                         log_thread_id();

                                         std::cout << "Clearing database\n";
                                         database.clear();
                                     });
                                     return true;
                                 }}};

    mm::menu_loop(menu);
}
