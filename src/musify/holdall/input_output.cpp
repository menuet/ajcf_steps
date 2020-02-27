
#include "input_output.hpp"
#include <iostream>

namespace musify { namespace io {

    std::string ask_question_get_string(std::string_view question)
    {
        std::cout << question << " ? ";
        std::string value{};
        std::cin >> value;
        return value;
    }

    char ask_question_get_char(std::string_view question)
    {
        std::cout << question << " ? ";
        char value{};
        std::cin >> value;
        return value;
    }

    int ask_question_get_int(std::string_view question)
    {
        std::cout << question << " ? ";
        int value{};
        std::cin >> value;
        return value;
    }

    double ask_question_get_double(std::string_view question)
    {
        std::cout << question << " ? ";
        double value{};
        std::cin >> value;
        return value;
    }

}} // namespace musify::io
