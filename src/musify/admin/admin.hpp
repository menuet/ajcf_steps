
#pragma once

#include <holdall/input_output.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace musify { namespace admin {

    constexpr char admin_name[] = "darkvador";
    constexpr int admin_pin_code = 1234;

    inline bool log_in()
    {
        std::cout << "Please log in\n";

        const auto name = io::ask_question_get_string("What is your name");

        const auto pin_code = io::ask_question_get_int("What is your pin code");

        return name == admin_name && pin_code == admin_pin_code;
    }

    inline std::filesystem::path check_command_line_arguments_and_get_database_file_path(int argc, char* argv[])
    {
        if (argc < 4)
            throw std::invalid_argument{
                "Please provide your name, your pin_code and the database file on the command line"};

        const std::string program_path = argv[0];
        std::cout << "Currently running this program: " << program_path << "\n";

        const std::string name = argv[1];
        const std::string pin_code_string = argv[2];
        const int pin_code = std::stoi(pin_code_string);
        if (name != admin_name || pin_code != admin_pin_code)
            throw std::invalid_argument{"You are not the administrator"};

        const std::string database_file_path_string = argv[3];
        return std::filesystem::path{database_file_path_string};
    }

}} // namespace musify::admin
