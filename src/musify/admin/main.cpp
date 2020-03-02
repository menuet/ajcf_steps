
#include "admin.hpp"
#include <holdall/database.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        const auto database_file_path =
            musify::admin::check_command_line_arguments_and_get_database_file_path(argc, argv);

        const auto database_lines = musify::database::ask_new_database_lines();

        if (!database_lines.empty())
        {
            std::cout << "Saving " << database_lines.size() << " lines to the database\n";

            musify::database::save_new_database_lines(database_file_path, database_lines);
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exceptional error occured: " << e.what() << '\n';
        return -1;
    }
}
