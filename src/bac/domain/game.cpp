
#include "game.hpp"
#include "logic.hpp"
#include <bac/util/menu.hpp>

namespace bac {

    void configure_options(std::ostream& out, std::istream& in, Options& options)
    {
        display_menu_and_configure(out, in, options);
    }

    void human_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                      [[maybe_unused]] const Options& options)
    {
        // TODO: ... call play(...)
        out << "\n!!! TODO Human vs Computer !!!\n\n";
    }

    void computer_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                         [[maybe_unused]] const Options& options)
    {
        // TODO: ... call play(...)
        out << "\n!!! TODO Computer vs Computer !!!\n\n";
    }

    void display_menu_and_play(std::ostream& out, std::istream& in)
    {
        Options options{};

        Menu menu{{'0', "Quit",
                   [&] {
                       out << "\nBye bye!\n";
                       return QuitMenu::Yes;
                   }},
                  {'1', "Configure Options",
                   [&] {
                       configure_options(out, in, options);
                       return QuitMenu::No;
                   }},
                  {'2', "Human vs Computer",
                   [&] {
                       human_plays_against_computer(out, in, options);
                       return QuitMenu::No;
                   }},
                  {'3', "Computer vs Computer", [&] {
                       computer_plays_against_computer(out, in, options);
                       return QuitMenu::No;
                   }}};

        auto display_title = [](std::ostream& out) { out << "Welcome to Bulls And Cows\nPlease select a menu option"; };

        menu_loop(display_title, out, in, menu);
    }

} // namespace bac
