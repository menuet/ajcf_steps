
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

    std::cout << "Thanks. Your account was successfully created:\n"
              << "Name: " << name << "\n"
              << "Password length: " << password.length() << "\n"
              << "Gender: " << gender << "\n"
              << "Age: " << age << " years\n"
              << "Size: " << size << " m\n";

    return 0;
}
