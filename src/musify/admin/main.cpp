
#include <holdall/input_output.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <string>

namespace musify {

    constexpr char admin_name[] = "darkvador";
    constexpr int admin_pin_code = 1234;

} // namespace musify

namespace mio = musify::io;

int main()
{
    std::cout << "Please log in\n";

    const auto name = mio::ask_question_get_string("What is your name");

    const auto pin_code = mio::ask_question_get_int("What is your pin code");

    if (name == musify::admin_name && pin_code == musify::admin_pin_code)
    {
        std::array<std::string, 5> artists_names{};
        unsigned int artists_count = 0U;

        auto yes_no = mio::ask_question_get_char("Would you like to add another artist to the database (Y/N)");
        bool add_another_artist = (yes_no == 'Y' || yes_no == 'y');

        while (add_another_artist)
        {
            artists_names[artists_count] = mio::ask_question_get_string("What is the new artist's name");
            ++artists_count;

            if (artists_count >= artists_names.size())
            {
                std::cout << "You cannot add more than " << artists_names.size() << " artists to the database\n";
                break;
            }

            yes_no = mio::ask_question_get_char("Would you like to add another artist to the database (Y/N)");
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
