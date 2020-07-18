
#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace musify::menu {

    struct Command
    {
        virtual ~Command() = default;
        virtual bool execute() = 0;
    };

    struct MenuOption
    {
        char shortcut;
        std::string description;
        std::shared_ptr<Command> command;
    };

    using Menu = std::vector<MenuOption>;

    inline void display_menu(const Menu& menu)
    {
        std::cout << "Please select an option\n";
        for (const auto& option : menu)
        {
            std::cout << option.shortcut << " -> " << option.description << "\n";
        }
    }

    inline Command* ask_user_choice(const Menu& menu)
    {
        char user_choice{};
        std::cin >> user_choice;

        const auto iter_option = std::find_if(menu.begin(), menu.end(), [&](const auto& option) {
            return std::toupper(option.shortcut) == std::toupper(user_choice);
        });

        return iter_option != menu.end() ? iter_option->command.get() : nullptr;
    }

    inline void menu_loop(const Menu& menu)
    {
        for (;;)
        {
            display_menu(menu);
            Command* user_choice_command = ask_user_choice(menu);
            if (user_choice_command)
            {
                if (!user_choice_command->execute())
                    return;
            }
            else
                std::cout << "Please enter a valid choice\n";
        }
    }

} // namespace musify::menu
