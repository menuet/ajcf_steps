
#include "codebreaker.hpp"
#include "logic.hpp"
#include <bac/util/input.hpp>
#include <chrono>
#include <iostream>
#include <thread>

namespace bac {

    using namespace std::literals;

    Code HumanCodebreaker::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                       const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        out << "Hey, codebreaker!\n";
        const auto attempt_number = attempts_and_feedbacks.size() + 1;
        while (true)
        {
            out << "What is your guess #" << (attempt_number < 10 ? "0" : "") << attempt_number;
            out << " (" << options.number_of_characters_per_code << " characters between '"
                << options.minimum_allowed_character << "' and '" << options.maximum_allowed_character << "')\n";
            out << "? ";
            Code attempt{ask_string(in)};
            if (validate_attempt(options, attempt))
                return attempt;
            out << "Your guess has an invalid length or contains non-allowed characters, please try again\n";
        }
    }

    Code ComputerCodebreaker::ask_attempt(std::ostream& out, std::istream&, const Options& options,
                                          const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
        {
            out << "[Codebreaker] Computing initial possible codes...\n";
            m_remaining_possible_codes = generate_all_possible_codes(options);
        }
        else
        {
            out << "[Codebreaker] Removing incompatibles codes among remaining possible codes...\n";
            const auto& last_attempt_and_feedback = attempts_and_feedbacks.back();
            remove_incompatible_codes_from_possible_solutions(last_attempt_and_feedback, m_remaining_possible_codes);
        }

        out << "[Codebreaker] Number of remaining possible codes: " << m_remaining_possible_codes.codes.size() << "\n";

        out << "[Codebreaker] Thinking ";

        for (unsigned int i = 0; i != 10; ++i)
        {
            std::this_thread::sleep_for(200ms);
            out << ". ";
        }

        auto attempt = pick_random_attempt(m_remaining_possible_codes);

        out << "[Codebreaker] Trying " << attempt.value << "\n";

        return attempt;
    }

} // namespace bac
