
// https://en.cppreference.com/w/cpp/language/initialization

#include <catch2/catch.hpp>
#include <limits>
#include <string>

namespace {

    namespace recommended_syntaxes {

        // Value-Initialisation with curly braces
        // (initialize with value 0 or calls user-defined default-constructor)
        int global_integer{};

        // Direct-Initialisation with curly braces
        int other_global_integer{1234};

        // List-Initialisation
        int global_array_of_integers[]{1, 2, 3, 4};

        TEST_CASE("recommended initialization syntaxes", "[initialization]")
        {
            REQUIRE(global_integer == 0);

            REQUIRE(other_global_integer == 1234);

            REQUIRE(global_array_of_integers[0] == 1);
            REQUIRE(global_array_of_integers[1] == 2);
            REQUIRE(global_array_of_integers[2] == 3);
            REQUIRE(global_array_of_integers[3] == 4);
        }

    } // namespace recommended_syntaxes

    namespace syntaxes_to_avoid {

        // Copy-Initialisation (C-style classic syntax)
        int yet_another_global_integer = 1234;

        // Direct-Initialisation with parenthesis (constructor-like syntax)
        int yet_another_global_integer_again(1234);

        // Warning: syntax gotcha:
        // declare a function returning an int
        // not a variable of type int !!!
        int not_an_integer_but_a_declaration_of_function_without_parameters_returning_int();

        // Aggregate-Initialisation
        int global_array_of_integers_again[] = {1, 2, 3, 4};

        // Default-Initialisation - may be dangerous
        // Safe for static storage variables
        // NOT SAFE for automatic storage variables (variables declared in functions or as class members)
        int g;               // g is global (i.e. has static storage), so its value is initialized to 0
        unsigned int ga[15]; // ga is global (i.e. has static storage), so its elements' values are initialized to 0

    } // namespace syntaxes_to_avoid

    TEST_CASE("uninitialized object of fundamental type", "[initialization]")
    {
        // Default-initialisation - DANGEROUS
        // i is a local to a function (i.e. has automatic storage), so it value is UNINITIALIZED and can be anything
        int i;
        // advice: use this syntax instead:
        // int i{};
        // or this one:
        // int i{0};

        // UNDEFINED BEHAVIOR!!!
        // One reads i whereas its value is uninitialized
        // One cannot tell much about value of i
        REQUIRE(i >= std::numeric_limits<int>::min());
        REQUIRE(i <= std::numeric_limits<int>::max());
    }

    class C
    {
    public:
        // Unsafe Default-Initialisation
        // i is a non-static class-data-member
        // its value may be unitialized when creating an object of class C !!!
        int i;
        // advice: use this syntax instead:
        // int i{};
        // or this one:
        // int i{0};

        int j{}; // safe always initialized to 0

        int k; // unsafe

        // safe too, because std::string's default constructor initializes the string to empty
        std::string s;
        // but, for consistency, you shoud write this: std::string s{};
    };

    TEST_CASE("uninitialized object of user-defined type", "[initialization]")
    {
        {
            // The C's default-constructor leaves member variable i uninitialized
            C c;

            // c.i contains garbage !!!
            REQUIRE(c.i >= std::numeric_limits<int>::min());
            REQUIRE(c.i <= std::numeric_limits<int>::max());

            REQUIRE(c.j == 0);

            // c.k contains garbage !!!
            REQUIRE(c.k >= std::numeric_limits<int>::min());
            REQUIRE(c.k <= std::numeric_limits<int>::max());

            REQUIRE(c.s == "");
        }

        {
            // C is a class without any constructor defined by the developer
            // One can use the safe Direct-Initialization syntax with curly braces
            // to initialize the data members with an explicit value.
            // All members are initialized with their respective provided value
            // If some values are not provided, the corresponding members are still initialized with 0
            C c{45};

            REQUIRE(c.i == 45);

            REQUIRE(c.j == 0);

            REQUIRE(c.k == 0);

            REQUIRE(c.s == "");
        }
    }

} // namespace
