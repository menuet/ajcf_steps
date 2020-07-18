
#include "browse_app.hpp"

int main(int argc, char* argv[])
{
    const auto database_file_path_opt = check_arguments(argc, argv);
    if (!database_file_path_opt)
        return -1;

    menu_loop(database_file_path_opt.value());

    return 0;
}
