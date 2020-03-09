
#include "database.impl.hpp"
#include "database.hpp"
#include "input_output.impl.hpp"
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

        const auto [line_type, remaining_text] = parse_until(line, '=');

        if (line_type == "Artist")
        {
            if (remaining_text.empty())
                return LoadingResult::IncompleteLine;
            database.artists.push_back(remaining_text);
            return LoadingResult::Ok;
        }

        if (line_type == "Album")
        {
            if (remaining_text.empty())
                return LoadingResult::IncompleteLine;
            database.albums.push_back(remaining_text);
            return LoadingResult::Ok;
        }

        if (line_type == "Song")
        {
            if (remaining_text.empty())
                return LoadingResult::IncompleteLine;
            database.songs.push_back(remaining_text);
            return LoadingResult::Ok;
        }

        return LoadingResult::UnknownLineType;
    }

    void display_music_entities(std::ostream& output_stream, const std::string& entity_type,
                                const std::vector<std::string>& music_entities)
    {
        output_stream << "-----------------\n";
        unsigned int entity_index = 0;
        for (const std::string& music_entity : music_entities)
        {
            output_stream << entity_type << " #" << ++entity_index << ": " << music_entity << "\n";
            output_stream << "-----------------\n";
        }
        output_stream << "--> " << music_entities.size() << " " << entity_type << "s\n";
        output_stream << "-----------------\n";
    }

}} // namespace musify::database
