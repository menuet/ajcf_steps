
#include "artists.hpp"
#include <holdall/input_output.hpp>
#include <fstream>
#include <iostream>

namespace musify { namespace artists {

    std::vector<std::string> ask_new_artists()
    {
        std::vector<std::string> artists_names{};

        auto yes_no = io::ask_question_get_char("Would you like to add another artist to the database (Y/N)");
        bool add_another_artist = (yes_no == 'Y' || yes_no == 'y');

        while (add_another_artist)
        {
            const std::string artist_name = io::ask_question_get_string("What is the new artist's name");
            artists_names.push_back(artist_name);

            yes_no = io::ask_question_get_char("Would you like to add another artist to the database (Y/N)");
            add_another_artist = (yes_no == 'Y' || yes_no == 'y');
        }

        return artists_names;
    }

    bool save_new_artists(const std::vector<std::string>& artists_names)
    {
        if (artists_names.empty())
            return true;

        std::ofstream artists_file{"artists.txt", std::ios::app};
        if (!artists_file)
        {
            std::cerr << "Error: could not create or open the file\n";
            return false;
        }

        std::cout << "Adding these artists to the database:\n";
        for (const std::string& artist_name : artists_names)
        {
            std::cout << artist_name << '\n';
            artists_file << artist_name << '\n';
        }

        return true;
    }

}} // namespace musify::artists
