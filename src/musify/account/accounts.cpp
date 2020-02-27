
#include "accounts.hpp"
#include "input_output.hpp"
#include <fmt/format.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

namespace musify { namespace accounts {

    Account create_account()
    {
        std::cout << "Please create your account.\n";

        Account account{};

        account.name = io::ask_question_get_string("What is your name");

        account.password = io::ask_question_get_string("What is your password");

        const auto gender_char = io::ask_question_get_char("What is your gender (M/F)");
        switch (gender_char)
        {
        case 'F':
            account.gender = Gender::Female;
            break;
        case 'M':
            account.gender = Gender::Male;
            break;
        default:
            account.gender = Gender::Female;
            break;
        }

        const int age = io::ask_question_get_int("What is your age (years)");
        if (age >= 0)
            account.age = static_cast<unsigned int>(age);
        else
            account.age = 99;

        account.size = io::ask_question_get_double("What is your size (meters)");

        return account;
    }

    void display_account(Account account)
    {
        const std::string gender_string = account.gender == Gender::Female ? "Female" : "Male";

        std::cout << "Thanks. Your account was successfully created:\n"
                  << "Name: " << account.name << "\n"
                  << "Password length: " << account.password.length() << "\n"
                  << "Gender: " << gender_string << "\n"
                  << "Age: " << account.age << " years\n"
                  << "Size: " << std::setprecision(2) << std::fixed << account.size << " m\n";
    }

}} // namespace musify::accounts
