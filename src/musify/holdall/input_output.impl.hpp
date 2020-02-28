
#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace musify { namespace io {

    inline std::string ask_question_get_string(std::ostream& output_stream, std::istream& input_stream,
                                               std::string_view question)
    {
        output_stream << question << " ? ";
        std::string value{};
        std::getline(input_stream, value);
        return value;
    }

    inline char ask_question_get_char(std::ostream& output_stream, std::istream& input_stream,
                                      std::string_view question)
    {
        const auto string_value = ask_question_get_string(output_stream, input_stream, question);
        std::istringstream iss(string_value);
        char value{};
        iss >> value;
        if (iss.fail() || iss.peek() != std::stringstream::traits_type::eof())
            throw std::domain_error{"Your input does not contain exactly one character"};
        return value;
    }

    inline int ask_question_get_int(std::ostream& output_stream, std::istream& input_stream, std::string_view question)
    {
        const auto string_value = ask_question_get_string(output_stream, input_stream, question);
        std::istringstream iss(string_value);
        int value{};
        iss >> value;
        if (iss.fail() || iss.peek() != std::stringstream::traits_type::eof())
            throw std::domain_error{"Your input is empty or contains some non-numeric characters"};
        return value;
    }

    inline double ask_question_get_double(std::ostream& output_stream, std::istream& input_stream,
                                          std::string_view question)
    {
        const auto string_value = ask_question_get_string(output_stream, input_stream, question);
        std::istringstream iss(string_value);
        double value{};
        iss >> value;
        if (iss.fail() || iss.peek() != std::stringstream::traits_type::eof())
            throw std::domain_error{"Your input is empty or contains some non-numeric characters"};
        return value;
    }

}} // namespace musify::io
