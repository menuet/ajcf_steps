
// https://en.cppreference.com/w/cpp/language/functions

#include <catch2/catch.hpp>

namespace {

    // Declaration of a functin
    // - which name is some_function
    // - qui returns a value of type int
    // - qui takes a first parameter c of type char
    // - qui takes a second parameter b of type bool
    int some_function(char c, bool b);
    // Because the function is declared,
    // Then the compiler accepts that one calls it later in the same source file.
    // If there is not any definition of this function in any source file
    // Then the linker emits an error

    // One can declare several time the same function
    // (note: one can avoid naming parameters in function's declarations)
    int some_function(char, bool b);

    // Definition of a function
    // - which name is call_another_function
    // - which does not return a result
    // - which takes one parameter my_ref_param de type int&
    void call_another_function(int& my_ref_param)
    {
        // one can call some_function because it was previously declared (but not yet defined)
        // the compiler checks that:
        // - the arguments and parameters types match
        // - the returned value's type and the assigned variable's type matches
        my_ref_param = some_function('a', true);
    }

    // Definition of the function some_function
    int some_function(char c, bool b)
    {
        if (b)
            return 123;
        else
            return static_cast<int>(c) - 5;
    }
    // The function some_function is not defined once, the linker will be happy

    // Definition of a function
    // - which name is fibonacci
    // - which return a value of type unsigned int
    // - which takes a parameter of type unsigned int
    unsigned int fibonacci(unsigned int n)
    {
        if (n == 0)
            return 0;
        if (n == 1)
            return 1;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
    // note: this function is recursive
    // somewhere in its body, it calls itself

    auto function_with_trailing_return_type() -> int
    {
        return 123;
    }

    TEST_CASE("functions", "[functions]")
    {
        const auto v1 = some_function('Z', true);

        REQUIRE(v1 == 123);

        const auto v2 = some_function('Z', false);

        REQUIRE(v2 == 85);

        const auto v3 = fibonacci(6);

        REQUIRE(v3 == 8);

        const auto v4 = function_with_trailing_return_type();

        REQUIRE(v4 == 123);
    }

} // namespace
