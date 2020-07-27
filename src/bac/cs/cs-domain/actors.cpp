
#include "actors.hpp"
#include <iostream>
#include <sstream>

namespace bac::cs::actors {

    void CodebreakerActor::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                       const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                                       std::function<void(Code)> callback)
    {
        m_actor.push_task([&out, &in, &options, attempts_and_feedbacks, callback, this] {
            callback(m_codebreaker.ask_attempt(out, in, options, attempts_and_feedbacks));
        });
    }

    void CodemakerActor::choose_secret_code(std::ostream& out, const Options& options,
                                            std::function<void(Code)> callback)
    {
        m_actor.push_task([&out, &options, callback, this] { callback(m_codemaker.choose_secret_code(out, options)); });
    }

    void CodemakerActor::compare_attempt_with_secret_code(std::ostream& out, const Code& attempt,
                                                          std::function<void(Feedback)> callback)
    {
        m_actor.push_task(
            [&out, attempt, callback, this] { callback(m_codemaker.compare_attempt_with_secret_code(out, attempt)); });
    }

    void GameActor::display(std::ostream& out, GameStatus status)
    {
        std::stringstream ss;
        display_board(ss, m_options, m_board,
                      status != GameStatus::Pending ? DisplaySecretCode::Yes : DisplaySecretCode::No);
        switch (status)
        {
        case GameStatus::Pending:
            break;
        case GameStatus::CodebreakerWon:
            ss << "Codebreaker Won!";
            break;
        case GameStatus::CodemakerWon:
            ss << "Codemaker Won!";
            break;
        }
        BAC_LOG(out, INFO, GAME, "Current Board:\n" << ss.str());
    }

    void GameActor::begin(CodemakerActor& codemaker_actor, std::ostream& out, const Options& options)
    {
        m_actor.push_task([&codemaker_actor, &out, options, this] {
            m_options = options;
            m_board.secret_code = Code{std::string(m_options.number_of_characters_per_code, '?')};
            m_board.attempts_and_feedbacks.clear();
            m_codemaker_actor = &codemaker_actor;
            m_codebreaker_actor = nullptr;
            display(out, GameStatus::Pending);
            m_codemaker_actor->choose_secret_code(out, m_options, [this](Code secret_code) {
                m_actor.push_task([secret_code, this] { m_board.secret_code = secret_code; });
            });
        });
    }

    void GameActor::begin(CodebreakerActor& codebreaker_actor, std::ostream& out, const Options& options)
    {
        m_actor.push_task([&codebreaker_actor, &out, options, this] {
            m_options = options;
            m_board.secret_code = Code{std::string(m_options.number_of_characters_per_code, '?')};
            m_board.attempts_and_feedbacks.clear();
            m_codebreaker_actor = &codebreaker_actor;
            m_codemaker_actor = nullptr;
            display(out, GameStatus::Pending);
        });
    }

    void GameActor::request_attempt(std::ostream& out, std::istream& in, std::optional<Feedback> feedback_opt,
                                    std::optional<Code> secret_code_opt, std::function<void(GameStatus, Code)> callback)
    {
        m_actor.push_task([&out, &in, feedback_opt, secret_code_opt, callback, this] {
            assert(m_codebreaker_actor != nullptr);
            if (feedback_opt)
            {
                assert(!m_board.attempts_and_feedbacks.empty());
                m_board.attempts_and_feedbacks.back().feedback = *feedback_opt;
                if (secret_code_opt)
                    m_board.secret_code = *secret_code_opt;
                const auto status = game_status(m_options, m_board);
                display(out, status);
                if (status != GameStatus::Pending)
                {
                    callback(status, Code{});
                    return;
                }
            }
            m_codebreaker_actor->ask_attempt(out, in, m_options, m_board.attempts_and_feedbacks,
                                             [callback, this](Code attempt) {
                                                 m_actor.push_task([attempt, callback, this] {
                                                     m_board.attempts_and_feedbacks.push_back({attempt});
                                                     callback(GameStatus::Pending, attempt);
                                                 });
                                             });
        });
    }

    void GameActor::request_feedback(std::ostream& out, Code attempt,
                                     std::function<void(Feedback, std::optional<Code>)> callback)
    {
        m_actor.push_task([&out, attempt, callback, this] {
            assert(m_codemaker_actor != nullptr);
            m_board.attempts_and_feedbacks.push_back({attempt});
            m_codemaker_actor->compare_attempt_with_secret_code(
                out, attempt, [&out, callback, this](Feedback feedback) {
                    m_actor.push_task([&out, feedback, callback, this] {
                        assert(!m_board.attempts_and_feedbacks.empty());
                        m_board.attempts_and_feedbacks.back().feedback = feedback;
                        const auto status = game_status(m_options, m_board);
                        display(out, status);
                        std::optional<Code> secret_code_opt;
                        if (status != GameStatus::Pending)
                            secret_code_opt = m_board.secret_code;
                        callback(feedback, secret_code_opt);
                    });
                });
        });
    }

} // namespace bac::cs::actors
