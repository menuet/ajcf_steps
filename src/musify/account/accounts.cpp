
#include "accounts.hpp"
#include <fmt/format.h>
#include <holdall/input_output.hpp>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>


namespace musify { namespace accounts {

    bool ask_name(std::string& name)
    {
        name = io::ask_question_get_string("What is your name");
        for (char c : name)
        {
            if (!std::isalpha(c) && !std::isblank(c))
            {
                std::cerr << "Error: Your name must contain only alphabetic characters or spaces\n";
                return false;
            }
        }
        return true;
    }

    bool ask_password(std::string& password)
    {
        password = io::ask_question_get_string("What is your password");
        for (char c : password)
        {
            if (!std::isalnum(c))
            {
                std::cerr << "Error: Your password must contain only alphanumeric characters\n";
                return false;
            }
        }
        return true;
    }

    bool ask_gender(Gender& gender)
    {
        const char gender_char = io::ask_question_get_char("What is your gender (M/F)");
        switch (gender_char)
        {
        case 'F':
        case 'f':
            gender = Gender::Female;
            break;
        case 'M':
        case 'm':
            gender = Gender::Male;
            break;
        default:
            std::cerr << "Error: Your gender must be either F (Female) or M (Male)\n";
            return false;
        }
        return true;
    }

    bool ask_age(unsigned int& always_positive_age)
    {
        const int negative_or_positive_age = io::ask_question_get_int("What is your age (years)");
        if (negative_or_positive_age < 18 || negative_or_positive_age > 99)
        {
            std::cerr << "Error: Your must be an adult, but not too old\n";
            return false;
        }
        always_positive_age = static_cast<unsigned int>(negative_or_positive_age);
        return true;
    }

    bool ask_size(double& size)
    {
        size = io::ask_question_get_double("What is your size (meters)");
        if (size < 1.5 || size > 2.5)
        {
            std::cerr << "Error: Your must not be too small or too tall\n";
            return false;
        }
        return true;
    }

    bool create_account(Account& account)
    {
        std::cout << "Please create your account.\n";

        if (!ask_name(account.name))
            return false;

        if (!ask_password(account.password))
            return false;

        if (!ask_gender(account.gender))
            return false;

        if (!ask_age(account.age))
            return false;

        if (!ask_size(account.size))
            return false;

        return true;
    }

    void display_account(const Account& account)
    {
        const std::string gender_string = account.gender == Gender::Female ? "Female" : "Male";

        std::cout << "Thanks. Your account was successfully created:\n"
                  << "Name: " << account.name << "\n"
                  << "Password length: " << account.password.length() << "\n"
                  << "Gender: " << gender_string << "\n"
                  << "Age: " << account.age << " years\n"
                  << "Size: " << std::setprecision(2) << std::fixed << account.size << " m\n";
    }

    void save_account(const Account& account)
    {
        std::ofstream accounts_file{"accounts.txt", std::ios::app};
        if (!accounts_file)
        {
            std::cerr << "Error: could not create or open the file\n";
            return;
        }

        const char gender_char = account.gender == Gender::Female ? 'F' : 'M';
        accounts_file << account.name << "," << account.password << "," << gender_char << "," << account.age << ","
                      << account.size << "\n";

        std::cout << "Thanks. Your account was successfully saved\n";
    }

}} // namespace musify::accounts
