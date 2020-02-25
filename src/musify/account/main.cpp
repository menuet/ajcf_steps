
#include <iostream>
#include <string>

namespace musify { namespace io {

    std::string ask_question_get_string(std::string question)
    {
        std::cout << question << " ? ";
        std::string value{};
        std::cin >> value;
        return value;
    }

    char ask_question_get_char(std::string question)
    {
        std::cout << question << " ? ";
        char value{};
        std::cin >> value;
        return value;
    }

    int ask_question_get_int(std::string question)
    {
        std::cout << question << " ? ";
        int value{};
        std::cin >> value;
        return value;
    }

    double ask_question_get_double(std::string question)
    {
        std::cout << question << " ? ";
        double value{};
        std::cin >> value;
        return value;
    }

}} // namespace musify::io

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
