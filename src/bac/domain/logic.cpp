
#include "logic.hpp"
#include "board.hpp"
#include "options.hpp"
#include <bac/util/random.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace bac {

    Code generate_random_secret_code(const Options& options)
    {
        Code secret_code{};
        std::generate_n(std::back_inserter(secret_code.value), options.number_of_characters_per_code, [&]() {
            return generate_random_value_between(options.minimum_allowed_character, options.maximum_allowed_character);
        });
        return secret_code;
    }

    bool validate_attempt(const Options& options, const Code& attempt)
    {
        if (attempt.value.length() != options.number_of_characters_per_code)
            return false;
        return std::all_of(attempt.value.begin(), attempt.value.end(), [&](const auto& c) {
            return c >= options.minimum_allowed_character && c <= options.maximum_allowed_character;
        });
    }

    Feedback compare_attempt_with_secret_code(const Code& attempt, const Code& secret_code)
    {
        assert(attempt.value.size() == secret_code.value.size());

        Feedback feedback{};

        const auto size = attempt.value.size();

        std::vector<bool> used_characters_in_attempt(size);
        std::vector<bool> used_characters_in_secret(size);

        // Check bulls
        for (unsigned int i = 0U; i != size; ++i)
        {
            if (attempt.value[i] == secret_code.value[i])
            {
                ++feedback.bulls;
                used_characters_in_attempt[i] = true;
                used_characters_in_secret[i] = true;
            }
        }

        // Check cows
        for (unsigned int a = 0U; a != size; ++a)
        {
            if (!used_characters_in_attempt[a])
            {
                for (unsigned int s = 0U; s != size; ++s)
                {
                    if (!used_characters_in_secret[s])
                    {
                        if (attempt.value[a] == secret_code.value[s])
                        {
                            ++feedback.cows;
                            used_characters_in_attempt[a] = true;
                            used_characters_in_secret[s] = true;
                            break;
                        }
                    }
                }
            }
        }

        return feedback;
    }

    bool has_codemaker_won(const Options& options, const Board& board)
    {
        return board.attempts_and_feedbacks.size() >= options.max_number_of_attempts &&
               board.attempts_and_feedbacks.back().feedback.bulls != options.number_of_characters_per_code;
    }

    bool has_codebreaker_won(const Options& options, const Board& board)
    {
        return board.attempts_and_feedbacks.size() != 0 &&
               board.attempts_and_feedbacks.back().feedback.bulls == options.number_of_characters_per_code;
    }

    GameStatus game_status(const Options& options, const Board& board)
    {
        if (board.attempts_and_feedbacks.size() != 0)
        {
            if (board.attempts_and_feedbacks.back().feedback.bulls == options.number_of_characters_per_code)
                return GameStatus::CodebreakerWon;
            if (board.attempts_and_feedbacks.size() >= options.max_number_of_attempts)
                return GameStatus::CodemakerWon;
        }
        return GameStatus::Pending;
    }

    void play(std::ostream& out, std::istream& in, const Options& options, State& state)
    {
        out << "\n#################################\nLet's play!\n";

        state.board.secret_code = state.codemaker.choose_secret_code(out, options);

        do
        {
            display_board(out, options, state.board, DisplaySecretCode::No);
            auto attempt = state.codebreaker->ask_attempt(out, in, options, state.board.attempts_and_feedbacks);
            auto feedback = state.codemaker.compare_attempt_with_secret_code(out, attempt);
            state.board.attempts_and_feedbacks.push_back(AttemptAndFeedback{std::move(attempt), std::move(feedback)});
        } while (!has_codemaker_won(options, state.board) && !has_codebreaker_won(options, state.board));

        display_board(out, options, state.board, DisplaySecretCode::Yes);

        if (has_codebreaker_won(options, state.board))
            out << "Codebreaker won!!!\n";
        else
            out << "Codebreaker lost!!!\n";
    }

} // namespace bac
