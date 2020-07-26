
#include "codebreaker.hpp"
#include "logic.hpp"
#include <bac/util/input.hpp>
#include <bac/util/logs.hpp>
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

    namespace {

        struct DurationLogger
        {
            std::ostream& out;
            std::chrono::system_clock::time_point start_time;
            DurationLogger(std::ostream& out) : out{out}, start_time{std::chrono::system_clock::now()} {};
            ~DurationLogger()
            {
                const auto duration =
                    std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time);
                BAC_LOG(out, INFO, CODEBREAKER, "Elapsed time: " << duration.count() << " s");
            }
        };

    } // namespace

    Code ComputerCodebreaker::ask_attempt(std::ostream& out, std::istream&, const Options& options,
                                          const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
        {
            BAC_LOG(out, INFO, CODEBREAKER, "[Codebreaker] Computing initial possible codes...");

            DurationLogger dl{out};
            m_remaining_possible_codes = generate_all_possible_codes(options);
        }
        else
        {
            BAC_LOG(out, INFO, CODEBREAKER,
                    "[Codebreaker] Removing incompatibles codes among remaining possible codes...");
            const auto& last_attempt_and_feedback = attempts_and_feedbacks.back();

            DurationLogger dl{out};
            remove_incompatible_codes_from_possible_solutions(options, last_attempt_and_feedback,
                                                              m_remaining_possible_codes);
        }

        BAC_LOG(out, INFO, CODEBREAKER,
                "Number of remaining possible codes: " << m_remaining_possible_codes.codes.size());

        BAC_LOG(out, INFO, CODEBREAKER, "Pretending to think");

        for (unsigned int i = 0; i != 5; ++i)
        {
            std::this_thread::sleep_for(200ms);
            BAC_LOG(out, INFO, CODEBREAKER, ".");
        }

        BAC_LOG(out, INFO, CODEBREAKER, "Choosing another code ...");

        Code attempt{};

        {
            DurationLogger dl{out};
            attempt = pick_random_attempt(options, attempts_and_feedbacks, m_remaining_possible_codes);
        }

        BAC_LOG(out, INFO, CODEBREAKER, "Trying " << attempt.value);

        return attempt;
    }

} // namespace bac
