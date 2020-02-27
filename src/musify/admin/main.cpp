
#include "admin.hpp"
#include "artists.hpp"
#include <iostream>

int main()
{
    if (!musify::admin::log_in())
    {
        std::cout << "Sorry, you are not the administrator of Musify\n";
        return -1;
    }

    const auto artists_names = musify::artists::ask_new_artists();

    musify::artists::save_new_artists(artists_names);

    return 0;
}
