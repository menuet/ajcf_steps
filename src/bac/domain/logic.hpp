
#pragma once

#include "board.hpp"
#include "codebreaker.hpp"
#include "codemaker.hpp"
#include <memory>

namespace bac {

    struct Options;

    struct State
    {
        std::unique_ptr<Codebreaker> codebreaker{};
        Codemaker codemaker{};
        Board board{};
    };

    // Create a randomly-generated secret code made of allowed characters
    Code generate_random_secret_code(const Options& options);

    // Validate that a codebreaker's attempt:
    // - Contains the required number characters
    // - Contains only allowed characters
    bool validate_attempt(const Options& options, const Code& attempt);

    // Compare a codebreaker's attempt with the secret code
    // and give the corresponding number of bulls and cows
    Feedback compare_attempt_with_secret_code(const Code& attempt, const Code& secret_code);

    // Test if the codemaker has won
    bool has_codemaker_won(const Options& options, const Board& board);

    // Test if the codebreaker has won
    bool has_codebreaker_won(const Options& options, const Board& board);

    enum class GameStatus
    {
        Pending,
        CodemakerWon,
        CodebreakerWon,
    };

    GameStatus game_status(const Options& options, const Board& board);

    // Play the game between the codemaker and the codebreaker
    // Algorithm:
    // - Ask the codemaker to choose a secret code
    // - DO
    //     - Display the board containing the list of codebreaker's attempts and codemaker's feedbacks
    //     - Ask the codebreaker to provide an attempt (based on previous codemaker's feedbacks)
    //     - Ask the codemaker to compare the attempt with its secret code and to give feedback
    //     - Add the attempt and the feedback to the list
    //   WHILE the codemaker has not won and the codebreaker has not won
    // - Display the board, the secret code, and who the winner is
    void play(std::ostream& out, std::istream& in, const Options& options, State& state);

} // namespace bac
