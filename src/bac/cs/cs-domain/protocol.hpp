
#pragma once

#include <string_view>
#include <bac/domain/board.hpp>
#include <bac/domain/logic.hpp>
#include <bac/domain/options.hpp>
#include <bac/util/logs.hpp>
#include <optional>
#include <string>
#include <utility>

namespace bac::cs::proto {

    enum class MessageId
    {
        Unknown = 0,
        MIN = 0,
        RequestBegin,
        RequestAttempt,
        RequestFeedback,
        MAX,
    };

    enum class PlayerRole
    {
        Codemaker,
        Codebreaker,
    };

    struct Payload_RequestBegin
    {
        static constexpr auto Id = MessageId::RequestBegin;
        static constexpr std::string_view Label = "RequestBegin";

        PlayerRole client_role{};
        Options options{};
    };

    struct Payload_RequestAttempt
    {
        static constexpr auto Id = MessageId::RequestAttempt;
        static constexpr std::string_view Label = "RequestAttempt";

        std::optional<Feedback> feedback_opt{};
        std::optional<Code> secret_code_opt{};
    };

    struct Payload_RequestFeedback
    {
        static constexpr auto Id = MessageId::RequestFeedback;
        static constexpr std::string_view Label = "RequestFeedback";

        Code attempt{};
    };

    template <typename PayloadT>
    std::string message_to_string(const PayloadT& payload);

    std::pair<MessageId, std::string_view> string_to_message_id(std::string_view msg_str);

    template <typename PayloadT>
    PayloadT string_to_payload(std::string_view payload_str);

    template <typename PayloadT, typename PayloadHandlerT>
    bool handle_payload(MessageId id, std::string_view payload_str, PayloadHandlerT payload_handler)
    {
        if (id != PayloadT::Id)
            return false;
        BAC_LOG_COUT(INFO, HANDLER, PayloadT::Label);
        const auto payload = string_to_payload<PayloadT>(payload_str);
        payload_handler(payload);
        return true;
    }

    template <typename PayloadHandlerT>
    void handle_message(std::string_view message, PayloadHandlerT payload_handler)
    {
        std::exception_ptr exptr{};
        const auto [id, payload_str] = proto::string_to_message_id(message);
        try
        {
            if (handle_payload<Payload_RequestBegin>(id, payload_str, payload_handler))
                return;
            if (handle_payload<Payload_RequestAttempt>(id, payload_str, payload_handler))
                return;
            if (handle_payload<Payload_RequestFeedback>(id, payload_str, payload_handler))
                return;
            throw std::runtime_error{"Unknown message"};
        }
        catch (std::exception&)
        {
            exptr = std::current_exception();
        }
        payload_handler(exptr);
    }

} // namespace bac::cs::proto
