
// https://en.cppreference.com/w/cpp/language/templates
// https://en.cppreference.com/w/cpp/language/template_parameters
// https://en.cppreference.com/w/cpp/language/static_assert
// https://en.cppreference.com/w/cpp/header/type_traits

#include <type_traits>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

namespace {

    // normal function
    std::string print(int i)
    {
        return fmt::format("function print({})", i);
    }

    // primary function template
    template <typename T>
    std::string print(const T& t)
    {
        static_assert(!std::is_class<T>::value);

        return fmt::format("instantiation of primary print<T>({})", t);
    }

    // function template specialization when T is std::string
    template <>
    std::string print<std::string>(const std::string& s)
    {
        return fmt::format("instantiation of specialized print<std::string>({})", s);
    }

    TEST_CASE("function templates", "[functions][templates]")
    {
        // call the function
        REQUIRE(print(123) == "function print(123)");

        // explicitely call the function template instantiation for type int
        REQUIRE(print<int>(123) == "instantiation of primary print<T>(123)");

        // implicitely call the function template instantiation for type double
        // (deduced by the compiler from the argument type)
        REQUIRE(print(12.45) == "instantiation of primary print<T>(12.45)");

        // implicitely call the function template instantiation for type char[6]
        // (deduced by the compiler from the argument type)
        REQUIRE(print("hello") == "instantiation of primary print<T>(hello)");

        // implicitely call the function template instantiation for type std::string
        // (deduced by the compiler from the argument type)
        // this calls the specialization
        REQUIRE(print(std::string{"hello"}) == "instantiation of specialized print<std::string>(hello)");

#if 0
        class C {};
        C c;
        REQUIRE(print(c) == ""); // compilation error because of static_assert : error C2607: static assertion failed
#endif
    }

    // primary class template
    template <typename T, typename U>
    struct ClassTemplate
    {
        std::string print()
        {
            return "ClassTemplate<T, U>::print()";
        }

        template <typename V>
        std::string member_function_template(V v)
        {
            return fmt::format("ClassTemplate<T, U>::member_function_template<V>({})", v);
        }
    };

    // partial specialization of class template for case T == U
    template <typename X>
    struct ClassTemplate<X, X>
    {
        std::string print()
        {
            return "ClassTemplate<X, X>::print()";
        }
    };

    // full specialization of class template for case T = int and U = bool
    template <>
    struct ClassTemplate<int, bool>
    {
        std::string print()
        {
            return "ClassTemplate<int, bool>::print()";
        }
    };

    // explicit instantation of class template, in order to force code generation
    template struct ClassTemplate<int, float>;

    TEST_CASE("class templates", "[classes][templates]")
    {
        // create an object which type is instantiated thanks to the primary template
        ClassTemplate<std::string, double> t1;

        REQUIRE(t1.print() == "ClassTemplate<T, U>::print()");

        // create an object which type is instantiated
        // thanks to the partial specialization where T == U
        ClassTemplate<bool, bool> t2;

        REQUIRE(t2.print() == "ClassTemplate<X, X>::print()");

        // create an object which type is instantiated
        // thanks to the full specialization where T == int and U == bool
        ClassTemplate<int, bool> t3;

        REQUIRE(t3.print() == "ClassTemplate<int, bool>::print()");

        // call the member function member_function_template<char>
        REQUIRE(t1.member_function_template('a') == "ClassTemplate<T, U>::member_function_template<V>(a)");
    }

    // Since C++14, one can define variable templates
    template <typename T>
    constexpr auto est_enum = std::is_enum<T>::value;

} // namespace
