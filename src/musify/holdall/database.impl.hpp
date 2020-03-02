
#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines(std::ostream& output_stream, std::istream& input_stream);

}} // namespace musify::database
