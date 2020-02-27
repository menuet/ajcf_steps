
#pragma once

#include <string>

namespace musify { namespace accounts {

    enum class Gender
    {
        Female,
        Male,
    };

    struct Account
    {
        std::string name{};
        std::string password{};
        Gender gender{};
        unsigned int age{};
        double size{};
    };

    bool create_account(Account& account);

    void display_account(Account account);

}} // namespace musify::accounts
