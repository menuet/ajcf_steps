
#pragma once

#include <filesystem>
#include <optional>

std::optional<std::filesystem::path> check_arguments(int argc, char* argv[]);

void menu_loop(const std::filesystem::path& database_file_path);
