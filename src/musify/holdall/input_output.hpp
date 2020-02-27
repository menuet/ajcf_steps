
#pragma once

#if !defined(MUSIFY_HOLDALL_INPUT_OUTPUT_HPP)
#define MUSIFY_HOLDALL_INPUT_OUTPUT_HPP

#include <string_view>
#include <string>

namespace musify { namespace io {

    std::string ask_question_get_string(std::string_view question);

    char ask_question_get_char(std::string_view question);

    int ask_question_get_int(std::string_view question);

    double ask_question_get_double(std::string_view question);

}} // namespace musify::io

#endif // MUSIFY_HOLDALL_INPUT_OUTPUT_HPP
