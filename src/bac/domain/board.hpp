
#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace bac {

    struct Options;

    // Code (secret code or attempt)
    struct Code
    {
        // Sequence of allowed characters
        std::string value{};
    };

    // Codemaker's Feedback
    struct Feedback
    {
        // Number of good and well-placed characters
        unsigned int bulls{};
        // Number of good but not well-placed characters
        unsigned int cows{};
    };

    // Codebreaker's attempt and associated codemaker's feedback
    struct AttemptAndFeedback
    {
        // Code Breaker's attempt
        Code attempt{};
        // Code Maker's feedback telling how many bulls and cows the attempts contains
        Feedback feedback{};
    };

    // Board containing:
    // - The "hidden" secret code
    // - The list of all codebreaker's attempts and codemaker's feedbacks
    struct Board
    {
        // Codemaker's secret code
        Code secret_code{};
        // Codebreaker's attempts and Codemaker's feedbacks
        std::vector<AttemptAndFeedback> attempts_and_feedbacks{};
    };

    enum class DisplaySecretCode
    {
        No,
        Yes,
    };

    // Display the list of attempts and feedbacks, and optionally the secret code
    void display_board(std::ostream& out, const Options& options, const Board& board,
                       DisplaySecretCode display_secret_code);

} // namespace bac
