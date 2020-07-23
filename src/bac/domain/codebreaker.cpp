
#include "codebreaker.hpp"
#include "logic.hpp"
#include <bac/util/input.hpp>
#include <iostream>

namespace bac {

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

} // namespace bac
