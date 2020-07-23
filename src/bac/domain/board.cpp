
#include "board.hpp"
#include "options.hpp"
#include <iostream>

namespace bac {

    static void display_row_of_dashes(std::ostream& out, unsigned int secret_code_length)
    {
        out << "----------";
        for (unsigned int i = 0U; i != secret_code_length; ++i)
            out << "--";
        out << "-----------------\n";
    }

    static void display_row_of_secret(std::ostream& out, const std::string& secret_code,
                                      DisplaySecretCode display_secret_code)
    {
        out << "| SECRET  ";
        if (display_secret_code == DisplaySecretCode::Yes)
            for (unsigned int i = 0U; i != secret_code.length(); ++i)
                out << " " << secret_code[i];
        else
            for (unsigned int i = 0U; i != secret_code.length(); ++i)
                out << " *";
        out << " |              |\n";
    }

    static void display_row_of_attempts_bulls_and_cows(std::ostream& out, unsigned int secret_code_length)
    {
        out << "| ATTEMPTS";
        for (unsigned int i = 0U; i != secret_code_length; ++i)
            out << "  ";
        out << " | BULLS | COWS |\n";
    }

    static void display_attempt_number(std::ostream& out, unsigned int attempt_number)
    {
        out << "| #" << (attempt_number < 10 ? "0" : "") << attempt_number << "     ";
    }

    static void display_row_of_not_yet_done_attempt(std::ostream& out, unsigned int attempt_number,
                                                    unsigned int secret_code_length)
    {
        display_attempt_number(out, attempt_number);
        for (unsigned int i = 0U; i != secret_code_length; ++i)
            out << " .";
        out << " |       |      |\n";
    }

    static void display_row_of_already_done_attempt(std::ostream& out, unsigned int attempt_number,
                                                    const AttemptAndFeedback& attempt_and_feedback)
    {
        display_attempt_number(out, attempt_number);
        for (unsigned int i = 0U; i != attempt_and_feedback.attempt.value.length(); ++i)
            out << " " << attempt_and_feedback.attempt.value[i];
        out << " |   ";
        out << attempt_and_feedback.feedback.bulls;
        out << "   |  ";
        out << attempt_and_feedback.feedback.cows;
        out << "   |\n";
    }

    void display_board(std::ostream& out, const Options& options, const Board& board,
                       DisplaySecretCode display_secret_code)
    {
        display_row_of_dashes(out, options.number_of_characters_per_code);

        display_row_of_secret(out, board.secret_code.value, display_secret_code);

        display_row_of_dashes(out, options.number_of_characters_per_code);

        display_row_of_attempts_bulls_and_cows(out, options.number_of_characters_per_code);

        display_row_of_dashes(out, options.number_of_characters_per_code);

        const auto current_attempts_count = static_cast<unsigned int>(board.attempts_and_feedbacks.size());

        for (unsigned int attempt_number = options.max_number_of_attempts; attempt_number > current_attempts_count;
             --attempt_number)
        {
            display_row_of_not_yet_done_attempt(out, attempt_number, options.number_of_characters_per_code);
        }

        for (unsigned int attempt_number = current_attempts_count; attempt_number > 0U; --attempt_number)
        {
            const auto& attempt_and_feedback = board.attempts_and_feedbacks[attempt_number - 1];
            display_row_of_already_done_attempt(out, attempt_number, attempt_and_feedback);
        }

        display_row_of_dashes(out, options.number_of_characters_per_code);
    }

} // namespace bac
