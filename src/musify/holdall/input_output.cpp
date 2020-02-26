
#include "input_output.hpp"
#include <iostream>

namespace musify { namespace io {

    std::string ask_question_get_string(const std::string& question)
    {
        std::cout << question << " ? ";
        std::string value{};
        std::cin >> value;
        return value;
    }

    char ask_question_get_char(const std::string& question)
    {
        std::cout << question << " ? ";
        char value{};
        std::cin >> value;
        return value;
    }

    int ask_question_get_int(const std::string& question)
    {
        std::cout << question << " ? ";
        int value{};
        std::cin >> value;
        return value;
    }

    double ask_question_get_double(const std::string& question)
    {
        std::cout << question << " ? ";
        double value{};
        std::cin >> value;
        return value;
    }

}} // namespace musify::io
