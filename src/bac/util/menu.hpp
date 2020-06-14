
#pragma once

#include "input.hpp"
#include <string_view>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace bac {

    enum class QuitMenu
    {
        No,
        Yes
    };

    using Command = std::function<QuitMenu()>;

    struct MenuOption
    {
        char shortcut;
        std::string description;
        Command command;
    };

    using Menu = std::vector<MenuOption>;

    inline void display_menu(std::function<void(std::ostream&)> display_title, std::ostream& out, const Menu& menu)
    {
        std::stringstream ss;
        display_title(ss);
        ss << "\n";
        for (const auto& option : menu)
        {
            ss << option.shortcut << " -> " << option.description << "\n";
        }
        out << ss.str();
    }

    inline Command ask_user_choice(std::istream& in, const Menu& menu)
    {
        const auto user_choice_opt = ask<char>(in);
        if (!user_choice_opt.has_value())
            return Command{};
        const auto& user_choice = user_choice_opt.value();

        const auto iter_option = std::find_if(menu.begin(), menu.end(), [&](const auto& option) {
            return std::toupper(option.shortcut) == std::toupper(user_choice);
        });

        return iter_option != menu.end() ? iter_option->command : Command{};
    }

    inline void menu_loop(std::function<void(std::ostream&)> display_title, std::ostream& out, std::istream& in,
                          const Menu& menu)
    {
        for (;;)
        {
            display_menu(display_title, out, menu);
            Command user_choice_command = ask_user_choice(in, menu);
            if (user_choice_command)
            {
                if (user_choice_command() == QuitMenu::Yes)
                    return;
            }
            else
                out << "Please enter a valid choice\n";
        }
    }

} // namespace bac
