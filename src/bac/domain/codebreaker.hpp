
#pragma once

#include "solver.hpp"

namespace bac {

    struct Options;
    struct Code;
    struct AttemptAndFeedback;

    class Codebreaker
    {
    public:
        virtual ~Codebreaker() = default;

        // Ask the codebreaker to give us an attempt
        virtual Code ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                 const std::vector<AttemptAndFeedback>& attempts_and_feedbacks) = 0;
    };

    // Codebreaker as a human
    class HumanCodebreaker : public Codebreaker
    {
    public:
        Code ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks) final;
    };

    // Codebreaker as computer (IA)
    class ComputerCodebreaker : public Codebreaker
    {
    public:
        Code ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks) final;

    private:
        PossibleCodes m_remaining_possible_codes{};
    };

} // namespace bac
