
#pragma once

#include <iostream>
#include <sstream>
#include <thread>

#define BAC_LOG(out, level, logger, args)                                                                              \
    {                                                                                                                  \
        std::stringstream ss__no_name_conflict__;                                                                      \
        ss__no_name_conflict__ << "[" << std::this_thread::get_id() << "-" #level "-" #logger "] " << args << "\n";    \
        out << ss__no_name_conflict__.str();                                                                           \
    }

#define BAC_LOG_COUT(level, logger, args) BAC_LOG(std::cout, level, logger, args)
