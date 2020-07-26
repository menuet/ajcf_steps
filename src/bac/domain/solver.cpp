
#include "solver.hpp"
#include "logic.hpp"
#include <bac/util/random.hpp>
#include <algorithm>
#include <cassert>
#ifdef __cpp_lib_execution
#include <execution>
#endif
#include <optional>

namespace bac {

    Code generate_code_with_all_characters(const Options& options)
    {
        char round_robin_char = options.minimum_allowed_character;
        Code code{};
        code.value.reserve(options.number_of_characters_per_code);
        for (unsigned int i = 0; i != options.number_of_characters_per_code; ++i)
        {
            code.value.push_back(round_robin_char);
            if (++round_robin_char > options.maximum_allowed_character)
                round_robin_char = options.minimum_allowed_character;
        }

        return code;
    }

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

        if (options.codebreaker_strategy == CodebreakerStrategy::Lazy)
            return possible_solutions;

        while (code.value != maximum_code.value)
        {
            possible_solutions.codes.push_back(code);
            generate_next_code(options, code);
        }
        possible_solutions.codes.push_back(code);

        return possible_solutions;
    }

    bool is_compatible_code(const Code& code, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        assert(!attempts_and_feedbacks.empty());
        const auto compatible = std::all_of(
            attempts_and_feedbacks.begin(), attempts_and_feedbacks.end(), [&](const auto& attempt_and_feedback) {
                const auto feedback = compare_attempt_with_secret_code(attempt_and_feedback.attempt, code);
                return feedback.bulls == attempt_and_feedback.feedback.bulls &&
                       feedback.cows == attempt_and_feedback.feedback.cows;
            });
        return compatible;
    }

    Code find_compatible_code(const Code& startup_code, const Options& options,
                              const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        auto code = startup_code;

        Code maximum_code = generate_maximum_code(options);

        while (code.value != maximum_code.value)
        {
            if (is_compatible_code(code, attempts_and_feedbacks))
                return code;
            generate_next_code(options, code);
        }

        if (is_compatible_code(code, attempts_and_feedbacks))
            return code;

        code = generate_minimum_code(options);

        while (code.value != startup_code.value)
        {
            if (is_compatible_code(code, attempts_and_feedbacks))
                return code;
            generate_next_code(options, code);
        }

        return startup_code;
    }

    Code pick_random_attempt(const Options& options, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                             const PossibleCodes& possible_solutions)
    {
        if (options.codebreaker_strategy == CodebreakerStrategy::Lazy)
        {
            if (attempts_and_feedbacks.empty())
                return generate_code_with_all_characters(options);
            return find_compatible_code(attempts_and_feedbacks.back().attempt, options, attempts_and_feedbacks);
        }

        using index_type = decltype(possible_solutions.codes.size());
        assert(possible_solutions.codes.size() > 0);
        const auto index =
            generate_random_value_between(static_cast<index_type>(0), possible_solutions.codes.size() - 1);
        return possible_solutions.codes[index];
    }

#ifdef __cpp_lib_execution
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
#else
    static void remove_incompatible_codes_without_policy(const AttemptAndFeedback& attempt_and_feedback,
                                                         PossibleCodes& possible_solutions)
    {
        const auto iter_new_end =
            std::remove_if(possible_solutions.codes.begin(), possible_solutions.codes.end(), [&](const auto& code) {
                const auto feedback = compare_attempt_with_secret_code(attempt_and_feedback.attempt, code);
                return feedback.bulls != attempt_and_feedback.feedback.bulls ||
                       feedback.cows != attempt_and_feedback.feedback.cows;
            });
        possible_solutions.codes.erase(iter_new_end, possible_solutions.codes.end());
    }
#endif

    void remove_incompatible_codes_from_possible_solutions(const Options& options,
                                                           const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleCodes& possible_solutions)
    {
        switch (options.codebreaker_strategy)
        {
#ifdef __cpp_lib_execution
        case CodebreakerStrategy::SimpleBruteForce:
            remove_incompatible_codes_with_policy(std::execution::seq, attempt_and_feedback, possible_solutions);
            break;
        case CodebreakerStrategy::ParallelBruteForce:
            remove_incompatible_codes_with_policy(std::execution::par, attempt_and_feedback, possible_solutions);
            break;
#else
        case CodebreakerStrategy::SimpleBruteForce:
        case CodebreakerStrategy::ParallelBruteForce:
            remove_incompatible_codes_without_policy(attempt_and_feedback, possible_solutions);
            break;
#endif
        case CodebreakerStrategy::Lazy:
            break;
        }
    }

} // namespace bac
