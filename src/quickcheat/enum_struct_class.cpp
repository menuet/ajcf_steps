
// https://en.cppreference.com/w/cpp/language/enum
// https://en.cppreference.com/w/cpp/language/class

#include <catch2/catch.hpp>
#include <sstream>
#include <string>

namespace {

    // unscoped enumeration (C syntax)
    enum Color_C
    {
        Color_C_White,       // implicitely 0
        Color_C_Blue = 1,    // explicitely 1
        Color_C_Red,         // implicitely Color_C_Blue + 1 (== 2)
        Color_C_Green = 5,   // explicitely 5
        Color_C_Magenta = 5, // explicitely 5 too, same value as Color_C_Green
    };

    // scoped enumeration (C++11 syntax)
    enum class Color_Cpp11
    {
        White,
        Blue = 1,
        Red,
        Green = 5,
        Magenta = 5,
    };

    std::string display_enum(Color_C color)
    {
        std::stringstream result;
        result << "color = " << static_cast<int>(color) << " (";
        switch (color)
        {
        case Color_C_White:
            result << "Color_C_White";
            break;
        case Color_C_Blue:
            result << "Color_C_Blue";
            break;
        case Color_C_Red:
            result << "Color_C_Red";
            break;
        case Color_C_Green: // cannot distinguish between Color_C_Green and Color_C_Magenta
            result << "Color_C_Green";
            break;
        }
        result << ")";
        return result.str();
    }

    std::string display_enum(Color_Cpp11 color)
    {
        std::stringstream result;
        result << "color = " << static_cast<int>(color) << " (";
        switch (color)
        {
        case Color_Cpp11::White:
            result << "Color_Cpp11::White";
            break;
        case Color_Cpp11::Blue:
            result << "Color_Cpp11::Blue";
            break;
        case Color_Cpp11::Red:
            result << "Color_Cpp11::Red";
            break;
        case Color_Cpp11::Green: // cannot distinguish between Color_Cpp11::Green and Color_Cpp11::Magenta
            result << "Color_Cpp11::Green";
            break;
        }
        result << ")";
        return result.str();
    }

    TEST_CASE("enumerations", "[enum]")
    {
        REQUIRE(display_enum(Color_C_Blue) == "color = 1 (Color_C_Blue)");

        REQUIRE(display_enum(Color_Cpp11::Green) == "color = 5 (Color_Cpp11::Green)");
    }

    // Differences between keywords struct/class:
    // None, except the default accessibilities:
    // In a struct, by default, members and inherited bases classes are public
    // In a class, by default, members and inherited bases classes are private

    // Convention: often we use the keyword struct
    // for trivial classes (i.e. which don't have any constructor/destructor and hold trivial members)
    // and which don't have any member functions

    struct S1
    {
        // implicitely public:
        int i;
        double d;
        void f()
        {
        }
    };

    struct S2 : /* implicitely public: */
                S1
    {
    };

    class C1
    {
        // implicitely private:
        int i;
        double d;
        void f()
        {
        }
    };

    struct C2 : /* implicitely private: */ S1
    {
    };

    // Special member functions:
    // Each of these function is automatically implemented by the compiler if possible
    // (even if the developer does not write them explicitely)
    // if the automatic implementation does not do what the developer wants then he needs to:
    // - implement it explicitely
    // - forbid its usage by using the syntax = delete
    // if the automatic implementation does do what the developer wants,
    // we can still be more explicit by using the syntax = default
    class C
    {
    public:
        // Default constructor
        C() = default;

        // Copy-constructor
        C(const C& c) = default;

        // Move-constructor
        C(C&& c) = default;

        // Destructor
        ~C() = default;

        // Copy-assignment
        C& operator=(const C& c) = default;

        // Move-assignment
        C& operator=(C&& c) = default;
    };

    TEST_CASE("special member functions", "[struct][class]")
    {
        // default-construction
        C c0;

        // copy-consutruction
        C c1(c0);

        // move-consutruction from a temporay unnamed object
        C c2(C{});

        // default-construction then copy-assignment
        C c3;
        c3 = c2;

        // default-construction then move-assignment
        C c4;
        c4 = std::move(c3);
    } // destruction of c4, c3, c2, c1, c0, in this order

} // namespace
