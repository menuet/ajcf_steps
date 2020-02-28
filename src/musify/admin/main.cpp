
#include "admin.hpp"
#include "artists.hpp"
#include <iostream>

int main()
{
    try
    {
        if (!musify::admin::log_in())
        {
            std::cout << "Sorry, you are not the administrator of Musify\n";
            return -1;
        }

        auto artists_names = musify::artists::ask_new_artists();

        musify::artists::sort_new_artists(artists_names);

        musify::artists::save_new_artists(artists_names);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exceptional error occured: " << e.what() << '\n';
    }
}
