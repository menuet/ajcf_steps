
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines();

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines);

    using Database = std::vector<std::string>;

    enum class LoadingResult
    {
        Ok = 0,
        FileNotFound,
        FileNotReadable,
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database);

    void display_database(const Database& database);

}} // namespace musify::database
