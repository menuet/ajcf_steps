
#include "server_app.hpp"
#include <bac/cs/cs-network/run.hpp>

int main()
{
    bac::cs::server::Application::Config config{{"127.0.0.1", 12345}};
    return bac::cs::net::run<bac::cs::server::Application>(std::move(config));
}
