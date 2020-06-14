
#pragma once

#include <iosfwd>

namespace bac {

    struct Options
    {
        unsigned int max_number_of_attempts{12};
        unsigned int number_of_characters_per_code{5};
        char minimum_allowed_character{'A'};
        char maximum_allowed_character{'H'};
    };

    // Display the menu for displaying/configuring/saving/loading the options
    void display_menu_and_configure(std::ostream& out, std::istream& in, Options& options);

    // Save the options in a file
    bool save_options(std::ostream& output_file_stream, const Options& options);

    // Load the options from a file
    bool load_options(std::istream& input_file_stream, Options& options);

} // namespace bac
