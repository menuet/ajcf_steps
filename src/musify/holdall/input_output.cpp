
#include "input_output.hpp"
#include "input_output.impl.hpp"

namespace musify { namespace io {

    std::string ask_question_get_string(std::string_view question)
    {
        return ask_question_get_string(std::cout, std::cin, question);
    }

    char ask_question_get_char(std::string_view question)
    {
        return ask_question_get_char(std::cout, std::cin, question);
    }

    int ask_question_get_int(std::string_view question)
    {
        return ask_question_get_int(std::cout, std::cin, question);
    }

    double ask_question_get_double(std::string_view question)
    {
        return ask_question_get_double(std::cout, std::cin, question);
    }

}} // namespace musify::io
