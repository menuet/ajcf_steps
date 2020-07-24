
#pragma once

#include "board.hpp"
#include "options.hpp"

namespace bac {

    // List of all possible codes that the IA has pre-computed
    struct PossibleCodes
    {
        std::vector<Code> codes;
    };

    // Pre-compute the list of all possible codes
    PossibleCodes generate_all_possible_codes(const Options& options);

    // Randomly choose the IA's attempt among the list of possible codes
    Code pick_random_attempt(const PossibleCodes& possible_solutions);

    // Given the codemaker's feedback,
    // Remove from all the possibles codes the ones that are not possible anymore
    void remove_incompatible_codes_from_possible_solutions(const Options& options,
                                                           const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleCodes& possible_solutions);

} // namespace bac
