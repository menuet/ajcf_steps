
#pragma once

#include <holdall/input_output.hpp>
#include <iostream>

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

}} // namespace musify::admin
