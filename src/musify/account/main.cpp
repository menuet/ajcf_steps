
#include "accounts.hpp"
#include "input_output.hpp"
#include <iostream>

int main()
{
    namespace mio = musify::io;

    std::cout << "Please create your account.\n";

    const auto name = mio::ask_question_get_string("What is your name");

    const auto password = mio::ask_question_get_string("What is your password");

    const auto gender = mio::ask_question_get_char("What is your gender (M/F)");

    const auto age = mio::ask_question_get_int("What is your age (years)");

    const auto size = mio::ask_question_get_double("What is your size (meters)");

    std::cout << "\nDisplay with cout\n";
    musify::accounts::display_account_with_cout(name, password, gender, age, size);

    std::cout << "\nDisplay with stringstream\n";
    musify::accounts::display_account_with_stringstream(name, password, gender, age, size);

    std::cout << "\nDisplay with printf\n";
    musify::accounts::display_account_with_printf(name, password, gender, age, size);

    std::cout << "\nDisplay with format\n";
    musify::accounts::display_account_with_format(name, password, gender, age, size);

    std::cout << "\nDisplay with format_to\n";
    musify::accounts::display_account_with_format_to(name, password, gender, age, size);

    return 0;
}
