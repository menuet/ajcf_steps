
#include "protocol.hpp"
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

namespace bac {

    static void to_json(json& j, const Options& options)
    {
        j = json{
            {"attempts", options.max_number_of_attempts},
            {"chars", options.number_of_characters_per_code},
            {"min", options.minimum_allowed_character},
            {"max", options.maximum_allowed_character},
            {"strategy", static_cast<int>(options.codebreaker_strategy)},
        };
    }

    static void from_json(const json& j, Options& options)
    {
        options.max_number_of_attempts = j.at("attempts").get<unsigned int>();
        options.number_of_characters_per_code = j.at("chars").get<unsigned int>();
        options.minimum_allowed_character = j.at("min").get<char>();
        options.maximum_allowed_character = j.at("max").get<char>();
        options.codebreaker_strategy = static_cast<CodebreakerStrategy>(j.at("strategy").get<unsigned int>());
    }

    static void to_json(json& j, const Code& code)
    {
        j = code.value;
    }

    static void from_json(const json& j, Code& code)
    {
        code.value = j.get<std::string>();
    }

    static void to_json(json& j, const Feedback& feedback)
    {
        j["bulls"] = feedback.bulls;
        j["cows"] = feedback.cows;
    }

    static void from_json(const json& j, Feedback& feedback)
    {
        feedback.bulls = j.at("bulls").get<unsigned int>();
        feedback.cows = j.at("cows").get<unsigned int>();
    }

#if 0
    static void to_json(json& j, GameStatus status)
    {
        switch (status)
        {
        case GameStatus::Pending:
            j = "Pending";
            break;
        case GameStatus::CodebreakerWon:
            j = "CodebreakerWon";
            break;
        case GameStatus::CodemakerWon:
            j = "CodemakerWon";
            break;
        }
    }

    static void from_json(const json& j, GameStatus& status)
    {
        const auto status_str = j.get<std::string>();
        if (status_str == "Pending")
            status = GameStatus::Pending;
        else if (status_str == "CodebreakerWon")
            status = GameStatus::CodebreakerWon;
        else if (status_str == "CodemakerWon")
            status = GameStatus::CodemakerWon;
    }
#endif

} // namespace bac

namespace bac::cs::proto {

    static void to_json(json& j, PlayerRole player_role)
    {
        switch (player_role)
        {
        case PlayerRole::Codemaker:
            j = "Codemaker";
            break;
        case PlayerRole::Codebreaker:
            j = "Codebreaker";
            break;
        }
    }

    static void from_json(const json& j, PlayerRole& player_role)
    {
        const auto player_role_str = j.get<std::string>();
        if (player_role_str == "Codemaker")
            player_role = PlayerRole::Codemaker;
        else if (player_role_str == "Codebreaker")
            player_role = PlayerRole::Codebreaker;
    }

    static void to_json(json& j, const Payload_RequestBegin& payload)
    {
        j["label"] = payload.Label;
        j["client_role"] = payload.client_role;
        j["options"] = payload.options;
    }

    static void from_json(const json& j, Payload_RequestBegin& payload)
    {
        payload.client_role = j.at("client_role").get<PlayerRole>();
        payload.options = j.at("options").get<Options>();
    }

    static void to_json(json& j, const Payload_RequestAttempt& payload)
    {
        j["label"] = payload.Label;
        if (payload.feedback_opt)
            j["feedback"] = *payload.feedback_opt;
        if (payload.secret_code_opt)
            j["secret_code"] = *payload.secret_code_opt;
    }

    static void from_json(const json& j, Payload_RequestAttempt& payload)
    {
        if (j.contains("feedback"))
            payload.feedback_opt = j.at("feedback").get<Feedback>();
        if (j.contains("secret_code"))
            payload.secret_code_opt = j.at("secret_code").get<Code>();
    }

    static void to_json(json& j, const Payload_RequestFeedback& payload)
    {
        j["label"] = payload.Label;
        j["attempt"] = payload.attempt;
    }

    static void from_json(const json& j, Payload_RequestFeedback& payload)
    {
        payload.attempt = j.at("attempt").get<Code>();
    }

    template <typename PayloadT>
    std::string message_to_string(const PayloadT& payload)
    {
        json j = payload;
        std::stringstream ss;
        ss << static_cast<int>(PayloadT::Id) << ':' << j;
        return ss.str();
    }

    std::pair<MessageId, std::string_view> string_to_message_id(std::string_view msg_str)
    {
        const auto colon_pos = msg_str.find(':');
        if (colon_pos == std::string_view::npos)
            return {MessageId::Unknown, msg_str};
        const auto msg_id_int = std::stoi(std::string{msg_str.substr(0, colon_pos)});
        if (msg_id_int <= static_cast<int>(MessageId::MIN) || msg_id_int >= static_cast<int>(MessageId::MAX))
            return {MessageId::Unknown, msg_str};
        return {static_cast<MessageId>(msg_id_int), msg_str.substr(colon_pos + 1)};
    }

    template <typename PayloadT>
    PayloadT string_to_payload(std::string_view payload_str)
    {
        auto j = json::parse(payload_str);
        return j.get<PayloadT>();
    }

    template Payload_RequestBegin string_to_payload<Payload_RequestBegin>(std::string_view);
    template std::string message_to_string<Payload_RequestBegin>(const Payload_RequestBegin&);

    template Payload_RequestAttempt string_to_payload<Payload_RequestAttempt>(std::string_view);
    template std::string message_to_string<Payload_RequestAttempt>(const Payload_RequestAttempt&);

    template Payload_RequestFeedback string_to_payload<Payload_RequestFeedback>(std::string_view);
    template std::string message_to_string<Payload_RequestFeedback>(const Payload_RequestFeedback&);

} // namespace bac::cs::proto
