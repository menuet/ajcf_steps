
#include "database.impl.hpp"
#include "input_output.impl.hpp"
#include "musical_factory.hpp"
#include <exception>
#include <fstream>

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines(std::ostream& output_stream, std::istream& input_stream)
    {
        std::vector<std::string> database_lines{};

        bool add_another_line = io::ask_question_get_yesno(output_stream, input_stream,
                                                           "Would you like to add another line to the database (Y/N)");

        while (add_another_line)
        {
            const std::string database_line = io::ask_question_get_string(
                output_stream, input_stream, "What is the new database line (please follow the database syntax)");
            database_lines.push_back(database_line);

            add_another_line = io::ask_question_get_yesno(output_stream, input_stream,
                                                          "Would you like to add another line to the database (Y/N)");
        }

        return database_lines;
    }

    std::pair<std::string, std::string> parse_until(const std::string& text, char separator)
    {
        const auto position_separator = text.find(separator);
        if (position_separator == std::string::npos)
            return {text, std::string{}};

        const auto text_before_separator = text.substr(0, position_separator);
        const auto text_after_separator = text.substr(position_separator + 1);
        return {text_before_separator, text_after_separator};
    }

    LoadingResult parse_and_load_database_line(const std::string& line, Database& database)
    {
        if (line.empty())
            return LoadingResult::Ok;

        if (line[0] == '#')
            return LoadingResult::Ok;

        const auto [thing_type_label, thing_data] = parse_until(line, '=');
        const auto [thing_name, thing_details] = parse_until(thing_data, ',');

        MusicalFactory& factory = singleton::Singleton<MusicalFactory>::get_instance();

        auto thing = factory.create_thing(thing_type_label, thing_name);
        if (!thing)
            return LoadingResult::UnknownLineType;

        switch (thing->parse_details(thing_details))
        {
        case ParsingResult::IncompleteData:
            return LoadingResult::IncompleteLine;
        case ParsingResult::InvalidData:
            return LoadingResult::ParsingError;
        default:
            break;
        }

        return database.insert_thing(std::move(thing)) == InsertionResult::Ok ? LoadingResult::Ok
                                                                              : LoadingResult::DuplicateThing;
    }

}} // namespace musify::database
