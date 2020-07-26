
#include "client_app.hpp"
#include <bac/cs/cs-network/run.hpp>
#include <optional>

static std::optional<bac::cs::client::Application::Config> check_arguments(int argc, char** argv)
{
    if (argc < 2)
        return std::nullopt;
    bac::cs::client::Application::Config config{{{"127.0.0.1", 12345}, bac::cs::net::AutoReconnect::Yes}};
    std::string_view player_role_str{argv[1]};
    if (player_role_str == "Codebreaker")
        config.player_role = bac::cs::proto::PlayerRole::Codebreaker;
    else if (player_role_str == "Codemaker")
        config.player_role = bac::cs::proto::PlayerRole::Codemaker;
    else
        return std::nullopt;
    return {std::move(config)};
}

int main(int argc, char** argv)
{
    const auto config_opt = check_arguments(argc, argv);
    if (!config_opt)
    {
        std::cerr
            << "Please provide a role as a command-line argument for the client: either Codebreaker or Codemaker\n";
        return -1;
    }

    auto config = *config_opt;

    return bac::cs::net::run<bac::cs::client::Application>(std::move(config));
}
