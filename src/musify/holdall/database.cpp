
#include "database.hpp"
#include "database.impl.hpp"
#include "input_output.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>

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
            const LoadingResult loading_result = parse_and_load_database_line(database_line, database);
            if (loading_result != LoadingResult::Ok)
                return loading_result;
        }
        return LoadingResult::Ok;
    }

    void Database::display() const
    {
        display_music_entities(std::cout, m_things);
    }

    const MusicalThing* Database::find_thing(const std::string& name, const std::type_info& thing_type) const
    {
        const auto [iter_begin, iter_end] = m_things.equal_range(name);
        const auto iter = std::find_if(iter_begin, iter_end, [&](const auto& name_and_thing) {
            return typeid(*name_and_thing.second) == thing_type;
        });
        return iter != iter_end ? iter->second.get() : nullptr;
    }

    Database::MusicalThings Database::find_things(const std::string& name) const
    {
        const auto [iter_begin, iter_end] = m_things.equal_range(name);
        MusicalThings things{};
        std::transform(iter_begin, iter_end, std::back_inserter(things),
                       [](const auto& name_and_thing) { return std::cref(*name_and_thing.second); });
        return things;
    }

    InsertionResult Database::insert_thing(std::unique_ptr<MusicalThing> thing)
    {
        if (find_thing(thing->name(), typeid(*thing)))
            return InsertionResult::DuplicateThing;
        m_things.emplace(thing->name(), std::move(thing));
        return InsertionResult::Ok;
    }

}} // namespace musify::database
