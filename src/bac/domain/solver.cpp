
#include "solver.hpp"
#include "logic.hpp"
#include <bac/util/random.hpp>
#include <algorithm>
#include <cassert>
#include <execution>

namespace bac {

    Code generate_minimum_code(const Options& options)
    {
        return Code{std::string(options.number_of_characters_per_code, options.minimum_allowed_character)};
    }

    Code generate_maximum_code(const Options& options)
    {
        return Code{std::string(options.number_of_characters_per_code, options.maximum_allowed_character)};
    }

    void generate_next_code(const Options& options, Code& code)
    {
        for (char& c : code.value)
        {
            if (c < options.maximum_allowed_character)
            {
                ++c;
                return;
            }
            else
                c = options.minimum_allowed_character;
        }
    }

    PossibleCodes generate_all_possible_codes(const Options& options)
    {
        Code code = generate_minimum_code(options);
        Code maximum_code = generate_maximum_code(options);

        PossibleCodes possible_solutions;

        while (code.value != maximum_code.value)
        {
            possible_solutions.codes.push_back(code);
            generate_next_code(options, code);
        }
        possible_solutions.codes.push_back(code);

        return possible_solutions;
    }

    Code pick_random_attempt(const PossibleCodes& possible_solutions)
    {
        using index_type = decltype(possible_solutions.codes.size());
        assert(possible_solutions.codes.size() > 0);
        const auto index =
            generate_random_value_between(static_cast<index_type>(0), possible_solutions.codes.size() - 1);
        return possible_solutions.codes[index];
    }

    template <typename ExecutionPolicyT>
    static void remove_incompatible_codes_with_policy(ExecutionPolicyT execution_policy,
                                                      const AttemptAndFeedback& attempt_and_feedback,
                                                      PossibleCodes& possible_solutions)
    {
        const auto iter_new_end = std::remove_if(
            execution_policy, possible_solutions.codes.begin(), possible_solutions.codes.end(), [&](const auto& code) {
                const auto feedback = compare_attempt_with_secret_code(attempt_and_feedback.attempt, code);
                return feedback.bulls != attempt_and_feedback.feedback.bulls ||
                       feedback.cows != attempt_and_feedback.feedback.cows;
            });
        possible_solutions.codes.erase(iter_new_end, possible_solutions.codes.end());
    }

    void remove_incompatible_codes_from_possible_solutions(const Options& options,
                                                           const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleCodes& possible_solutions)
    {
        if (options.codebreaker_strategy == CodebreakerStrategy::ParallelBruteForce)
            remove_incompatible_codes_with_policy(std::execution::par, attempt_and_feedback, possible_solutions);
        else
            remove_incompatible_codes_with_policy(std::execution::seq, attempt_and_feedback, possible_solutions);
    }

} // namespace bac
