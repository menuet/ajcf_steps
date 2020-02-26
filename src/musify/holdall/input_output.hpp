
#pragma once

#if !defined(MUSIFY_HOLDALL_INPUT_OUTPUT_HPP)
#define MUSIFY_HOLDALL_INPUT_OUTPUT_HPP

#include <string>

namespace musify { namespace io {

    std::string ask_question_get_string(const std::string& question);

    char ask_question_get_char(const std::string& question);

    int ask_question_get_int(const std::string& question);

    double ask_question_get_double(const std::string& question);

}} // namespace musify::io

#endif // MUSIFY_HOLDALL_INPUT_OUTPUT_HPP
