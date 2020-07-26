
#pragma once

#include <asio/signal_set.hpp>
#include <bac/util/logs.hpp>

namespace bac::cs::net {

    template <typename ApplicationT>
    int run(typename ApplicationT::Config config)
    {
        try
        {
            asio::io_context io_context;

            asio::signal_set signals(io_context, SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto) {
                BAC_LOG_COUT(WARN, RUN, "Termination requested");
                io_context.stop();
            });

            ApplicationT application(std::move(config), io_context);

            if (!application.start())
            {
                BAC_LOG_COUT(ERROR, RUN, "Startup failed");
                return -1;
            }

            io_context.run();
        }
        catch (std::exception& ex)
        {
            BAC_LOG_COUT(ERROR, RUN, "Exception: " << ex.what());
            return -1;
        }

        return 0;
    }

} // namespace bac::cs::net
