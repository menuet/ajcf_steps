
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines();

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines);

}} // namespace musify::database
