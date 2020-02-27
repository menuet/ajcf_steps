
#pragma once

#include <string>

namespace musify { namespace accounts {

    void display_account_with_cout(std::string name, std::string password, char gender, int age, double size);

    void display_account_with_stringstream(std::string name, std::string password, char gender, int age, double size);

    void display_account_with_printf(std::string name, std::string password, char gender, int age, double size);

    void display_account_with_format(std::string name, std::string password, char gender, int age, double size);

    void display_account_with_format_to(std::string name, std::string password, char gender, int age, double size);

}} // namespace musify::accounts
