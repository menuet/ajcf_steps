
// https://en.cppreference.com/w/cpp/language/switch
// https://en.cppreference.com/w/cpp/language/if
// https://en.cppreference.com/w/cpp/language/operator_other
// https://en.cppreference.com/w/cpp/language/while
// https://en.cppreference.com/w/cpp/language/do
// https://en.cppreference.com/w/cpp/language/for
// https://en.cppreference.com/w/cpp/language/range-for

#include <catch2/catch.hpp>
#include <sstream>
#include <string>

namespace {

    std::string example_switch(int value)
    {
        std::string result{};

        switch (value)
        {
        case 1:
            // if value equals 1 then go here
            result += "choice 1!";
            break; // will exit the switch block
        case 2:
            // if value equals 2 then go here
            result += "choice 2!";
            // will not exit the switch block and will execute the case 3 instructions
            // In order to tell the reader that we really want to fallthrough, we can use this attribute
            [[fallthrough]];
        case 3:
            // if value equals 3 then go here
            result += "choice 3!";
            break; // will exit the switch block
        default:
            // if value is not 1, 2 or 3 then go here
            result += "other choice!";
            break;
        }

        return result;
    }

    TEST_CASE("switch", "[conditions]")
    {
        REQUIRE(example_switch(1) == "choice 1!");

        REQUIRE(example_switch(2) == "choice 2!choice 3!");

        REQUIRE(example_switch(3) == "choice 3!");

        REQUIRE(example_switch(42) == "other choice!");
    }

    std::string example_if(std::string sound, int legs_count)
    {
        std::string result;

        if (sound == "miaou" || sound == "ouaf")
        {
            result += "dogs and cats";
        }
        else if (sound == "cuicui" && legs_count == 1)
        {
            result += "pink floyds";
        }
        else
        {
            result += "others";
            if (legs_count == 2)
            {
                result += "bipeds";
            }
        }

        return result;
    }

    TEST_CASE("if", "[conditions]")
    {
        REQUIRE(example_if("miaou", 123) == "dogs and cats");

        REQUIRE(example_if("cuicui", 1) == "pink floyds");

        REQUIRE(example_if("cuicui", 2) == "othersbipeds");

        REQUIRE(example_if("meuh", 4) == "others");
    }

    std::string example_ternary_operator(std::string sound)
    {
        return (sound == "meuh") ? "This is a cow" : "This is NOT a cow";
    }

    TEST_CASE("ternary operator ? : ", "[conditions]")
    {
        REQUIRE(example_ternary_operator("meuh") == "This is a cow");

        REQUIRE(example_ternary_operator("grrr") == "This is NOT a cow");
    }

    std::string example_while_do()
    {
        std::stringstream result{};

        int i = 5;

        while (i > 0) // if condition is true, then we enter the loop body
        {
            result << i << ",";
            i = i - 1;
            if (i % 3 == 0)
                // we can exit from the loop sooner thanks to break
                break;
        } // here, we go back to while and re-check the condition and possibly
          // re-enter the loop body

        result << "exited first loop,";

        do // we always enter the loop body at least once
        {
            result << i << ",";
            i = i + 1;
            if (i == 5)
                // we can go to the beginning of the loop body thanks to continue
                continue;
        } while (i < 5); // here we test the condition to decide
                         // whether we continue or not

        result << "exited second loop.";

        return result.str();
    }

    TEST_CASE("while / do", "[loops]")
    {
        REQUIRE(example_while_do() == "5,4,exited first loop,3,4,exited second loop.");
    }

    std::string example_for()
    {
#if 0
        // general syntax :
        for (initialisation; condition; modification)
        {
            instructions;
        }

        // equivalent to :
        initialisation;
        while (condition)
        {
            instructions;
            modification;
        }
#endif

        std::stringstream result{};

        // example :
        for (int i = 1; i != 5; i = i + 1)
        {
            result << i;
            result << ",";
        }

        result << "exited first loop,";

        // equivalent to :
        {
            int i = 1;     // initialisation
            while (i != 5) // condition
            {
                // instructions
                result << i;
                result << ",";

                i = i + 1; // modification
            }
        }

        result << "exited second loop.";

        return result.str();
    }

    TEST_CASE("for", "[loops]")
    {
        REQUIRE(example_for() == "1,2,3,4,exited first loop,1,2,3,4,exited second loop.");
    }

    std::string example_range_for()
    {
#if 0
        // general syntax :
        for (type_element var_element : collections_elements)
        {
            instructions that use var_element;
        }
#endif

        std::stringstream result{};

        // a string is a collection of characters
        std::string s = "hello";

        // therefore we can iterate over each character
        for (char c : s)
        {
            result << '[' << c << "]";
        }

        return result.str();
    }

    TEST_CASE("range-for", "[loops]")
    {
        REQUIRE(example_range_for() == "[h][e][l][l][o]");
    }

} // namespace
