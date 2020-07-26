
#pragma once

#include <bac/domain/codebreaker.hpp>
#include <bac/domain/codemaker.hpp>
#include <bac/domain/logic.hpp>
#include <bac/util/actor.hpp>
#include <bac/util/logs.hpp>
#include <optional>

namespace bac::cs::actors {

    class CodebreakerActor
    {
        Actor m_actor;
        ComputerCodebreaker m_codebreaker;

    public:
        void ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                         std::function<void(Code)> callback);
    };

    class CodemakerActor
    {
        Actor m_actor;
        Codemaker m_codemaker;

    public:
        void choose_secret_code(std::ostream& out, const Options& options, std::function<void(Code)> callback);

        void compare_attempt_with_secret_code(std::ostream& out, const Code& attempt,
                                              std::function<void(Feedback)> callback);
    };

    class GameActor
    {
        Actor m_actor;
        Options m_options;
        Board m_board;
        CodemakerActor* m_codemaker_actor{};
        CodebreakerActor* m_codebreaker_actor{};

        void display(std::ostream& out, GameStatus status);

    public:
        void begin(CodemakerActor& codemaker_actor, std::ostream& out, const Options& options);

        void begin(CodebreakerActor& codebreaker_actor, std::ostream& out, const Options& options);

        void request_attempt(std::ostream& out, std::istream& in, std::optional<Feedback> feedback_opt,
                             std::optional<Code> secret_code_opt, std::function<void(GameStatus, Code)> callback);

        void request_feedback(std::ostream& out, Code attempt,
                              std::function<void(Feedback, std::optional<Code>)> callback);
    };

} // namespace bac::cs::actors
