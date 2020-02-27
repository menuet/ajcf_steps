
#pragma once

#include <iostream>

namespace musify { namespace io {

    inline std::string ask_question_get_string(std::ostream& output_stream, std::istream& input_stream,
                                               std::string_view question)
    {
        output_stream << question << " ? ";
        std::string value{};
        input_stream >> value;
        return value;
    }

    inline char ask_question_get_char(std::ostream& output_stream, std::istream& input_stream,
                                      std::string_view question)
    {
        output_stream << question << " ? ";
        char value{};
        input_stream >> value;
        return value;
    }

    inline int ask_question_get_int(std::ostream& output_stream, std::istream& input_stream, std::string_view question)
    {
        output_stream << question << " ? ";
        int value{};
        input_stream >> value;
        return value;
    }

    inline double ask_question_get_double(std::ostream& output_stream, std::istream& input_stream,
                                          std::string_view question)
    {
        output_stream << question << " ? ";
        double value{};
        input_stream >> value;
        return value;
    }

}} // namespace musify::io
