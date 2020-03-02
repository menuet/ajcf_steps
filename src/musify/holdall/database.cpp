
#include "database.hpp"
#include "database.impl.hpp"
#include "input_output.hpp"
#include <fstream>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines()
    {
        return ask_new_database_lines(std::cout, std::cin);
    }

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines)
    {
        std::ofstream database_file{database_file_path.string(), std::ios::app};
        if (!database_file)
            throw std::runtime_error{"Error: could not create or open the file"};

        for (const auto& database_line : database_lines)
            database_file << database_line << '\n';
    }

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database)
    {
        if (!std::filesystem::is_regular_file(database_file_path))
            return LoadingResult::FileNotFound;
        std::ifstream ifs{database_file_path.string()};
        if (ifs.fail())
            return LoadingResult::FileNotReadable;
        std::string database_line;
        while (std::getline(ifs, database_line))
        {
            if (!database_line.empty())
                database.push_back(database_line);
        }
        return LoadingResult::Ok;
    }

    void display_database(const Database& database)
    {
        std::cout << "-----------------\n";
        unsigned int line_index = 0;
        for (const std::string& database_line : database)
        {
            std::cout << "Line #" << ++line_index << ": " << database_line << "\n";
            std::cout << "-----------------\n";
        }
        std::cout << "--> " << database.size() << " lines\n";
        std::cout << "-----------------\n";
    }

}} // namespace musify::database
