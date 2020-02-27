
#include "accounts.hpp"
#include <iostream>

int main()
{
    musify::accounts::Account account{};

    if (!musify::accounts::create_account(account))
    {
        std::cerr << "Error: Something went wrong!\n";
        return -1;
    }

    musify::accounts::display_account(account);

    musify::accounts::save_account(account);

    return 0;
}
