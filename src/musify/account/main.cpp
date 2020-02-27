
#include "accounts.hpp"

int main()
{
    const auto account = musify::accounts::create_account();

    musify::accounts::display_account(account);

    return 0;
}
