
#include <array>
#include <fstream>
#include <iostream>
#include <string>

namespace musify {

    constexpr char admin_name[] = "darkvador";
    constexpr int admin_pin_code = 1234;

} // namespace musify

int main()
{
    std::cout << "Please log in\n";

    std::cout << "What is your name ? ";
    std::string name{};
    std::cin >> name;

    std::cout << "What is your pin code ? ";
    int pin_code{};
    std::cin >> pin_code;

    if (name == musify::admin_name && pin_code == musify::admin_pin_code)
    {
        std::array<std::string, 5> artists_names{};
        unsigned int artists_count = 0U;

        std::cout << "Would you like to add another artist to the database (Y/N) ? ";
        char yes_no{};
        std::cin >> yes_no;
        bool add_another_artist = (yes_no == 'Y' || yes_no == 'y');

        while (add_another_artist)
        {
            std::cout << "What is the new artist's name ? ";
            std::string artist_name;
            std::cin >> artist_name;
            artists_names[artists_count] = artist_name;
            ++artists_count;

            if (artists_count >= artists_names.size())
            {
                std::cout << "You cannot add more than " << artists_names.size() << " artists to the database\n";
                break;
            }

            std::cout << "Would you like to add another artist to the database (Y/N) ? ";
            std::cin >> yes_no;
            add_another_artist = (yes_no == 'Y' || yes_no == 'y');
        }

        std::ofstream artists_file{"artists.txt", std::ios::app};
        if (!artists_file)
        {
            std::cerr << "Error: could not create or open the file\n";
            return -2;
        }

        std::cout << "Adding these artists to the database:\n";
        for (unsigned int artist_index = 0U; artist_index < artists_count; ++artist_index)
        {
            const std::string& artist_name = artists_names[artist_index];
            std::cout << artist_name << '\n';
            artists_file << artist_name << '\n';
        }

        return 0;
    }
    else
    {
        std::cout << "Sorry, you are not the administrator of Musify\n";
        return -1;
    }
}
