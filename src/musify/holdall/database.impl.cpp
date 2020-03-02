
#include "database.impl.hpp"
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

}} // namespace musify::database
