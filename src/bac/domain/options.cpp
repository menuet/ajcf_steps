
#include "options.hpp"
#include <bac/util/menu.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace bac {

    void display_menu_and_configure(std::ostream& out, std::istream& in, Options& options)
    {
        Menu menu{{'0', "Back to main menu", [&] { return QuitMenu::Yes; }},
                  {'1', "Modify Maximum number of attempts per game",
                   [&] {
                       out << "Enter the Maximum number of attempts: ";
                       options.max_number_of_attempts = ask<unsigned int>(in).value_or(options.max_number_of_attempts);
                       return QuitMenu::No;
                   }},
                  {'2', "Modify Number of characters in a code",
                   [&] {
                       out << "Enter the Number of character per code: ";
                       options.number_of_characters_per_code =
                           ask<unsigned int>(in).value_or(options.number_of_characters_per_code);
                       return QuitMenu::No;
                   }},
                  {'3', "Modify Minimum allowed character",
                   [&] {
                       out << "Enter the Minimum allowed character: ";
                       options.minimum_allowed_character = ask<char>(in).value_or(options.minimum_allowed_character);
                       return QuitMenu::No;
                   }},
                  {'4', "Modify Maximum allowed character",
                   [&] {
                       out << "Enter the Maximum allowed character: ";
                       options.maximum_allowed_character = ask<char>(in).value_or(options.maximum_allowed_character);
                       return QuitMenu::No;
                   }},
                  {'5', "Save options",
                   [&] {
                       std::ofstream file_output_stream{"options.txt"};
                       save_options(file_output_stream, options);
                       return QuitMenu::No;
                   }},
                  {'6', "Load options", [&] {
                       std::ifstream file_input_stream{"options.txt"};
                       load_options(file_input_stream, options);
                       return QuitMenu::No;
                   }}};

        auto display_title = [&](std::ostream& out) {
            out << "Here are the current options:\n";
            out << "Maximum number of attempts per game: " << options.max_number_of_attempts << "\n";
            out << "Number of characters in a code: " << options.number_of_characters_per_code << "\n";
            out << "Range of allowed characters: from '" << options.minimum_allowed_character << "' to '"
                << options.maximum_allowed_character << "'\n";
            out << "Please select a menu option to configure the game options";
        };

        menu_loop(display_title, out, in, menu);
    }

    bool save_options(std::ostream& output_file_stream, const Options& options)
    {
        output_file_stream << "max_number_of_attempts=" << options.max_number_of_attempts << "\n";
        output_file_stream << "number_of_characters_per_code=" << options.number_of_characters_per_code << "\n";
        output_file_stream << "minimum_allowed_character=" << options.minimum_allowed_character << "\n";
        output_file_stream << "maximum_allowed_character=" << options.maximum_allowed_character << "\n";
        return output_file_stream.good();
    }

    bool load_options(std::istream& input_file_stream, Options& options)
    {
        std::string line{};
        while (std::getline(input_file_stream, line))
        {
            std::istringstream iss{line};
            std::string field_name{};
            std::getline(iss, field_name, '=');
            if (field_name == "max_number_of_attempts")
                iss >> options.max_number_of_attempts;
            else if (field_name == "number_of_characters_per_code")
                iss >> options.number_of_characters_per_code;
            else if (field_name == "minimum_allowed_character")
                iss >> options.minimum_allowed_character;
            else if (field_name == "maximum_allowed_character")
                iss >> options.maximum_allowed_character;
        }
        return input_file_stream.good() || input_file_stream.eof();
    }

} // namespace bac
