
#include "accounts.hpp"
#include <fmt/format.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

namespace musify { namespace accounts {

    void display_account_with_cout(std::string name, std::string password, char gender, int age, double size)
    {
        std::cout << "Thanks. Your account was successfully created:\n"
                  << "Name: " << name << "\n"
                  << "Password length: " << password.length() << "\n"
                  << "Gender: " << gender << "\n"
                  << "Age: " << age << " years\n"
                  << "Size: " << std::setprecision(2) << std::fixed << size << " m\n";
    }

    void display_account_with_stringstream(std::string name, std::string password, char gender, int age, double size)
    {
        std::stringstream ss;
        ss << "Thanks. Your account was successfully created:\n"
           << "Name: " << name << "\n"
           << "Password length: " << password.length() << "\n"
           << "Gender: " << gender << "\n"
           << "Age: " << age << " years\n"
           << "Size: " << std::setprecision(2) << std::fixed << size << " m\n";
        const std::string formatted_text = ss.str();
        std::cout << formatted_text;
    }

    void display_account_with_printf(std::string name, std::string password, char gender, int age, double size)
    {
        std::printf(
            "Thanks. Your account was successfully created:\n"
            "Name: %s\n"
            "Password length: %zu\n"
            "Gender: %c\n"
            "Age: %i years\n"
            "Size: %.2f m\n",
            name.c_str(), password.length(), gender, age, size);
    }

    void display_account_with_format(std::string name, std::string password, char gender, int age, double size)
    {
        const std::string formatted_text = fmt::format(
            "Thanks. Your account was successfully created:\n"
            "Name: {}\n"
            "Password: {:*^{}}\n"
            "Gender: {}\n"
            "Age: {} years\n"
            "Size: {:.2f} m\n",
            name, "", password.length(), gender, age, size);
        std::cout << formatted_text;
    }

    void display_account_with_format_to(std::string name, std::string password, char gender, int age, double size)
    {
        fmt::format_to(std::ostream_iterator<char>(std::cout),
                       "Thanks. Your account was successfully created:\n"
                       "Name: {}\n"
                       "Password: {:*^{}}\n"
                       "Gender: {}\n"
                       "Age: {} years\n"
                       "Size: {:.2f} m\n",
                       name, "", password.length(), gender, age, size);
    }

}} // namespace musify::accounts
