
#pragma once

#if !defined(MUSIFY_ACCOUNT_INPUT_OUTPUT_HPP)
#define MUSIFY_ACCOUNT_INPUT_OUTPUT_HPP

#include <string>

namespace musify { namespace io {

    std::string ask_question_get_string(std::string question);

    char ask_question_get_char(std::string question);

    int ask_question_get_int(std::string question);

    double ask_question_get_double(std::string question);

}} // namespace musify::io

#endif // MUSIFY_ACCOUNT_INPUT_OUTPUT_HPP
