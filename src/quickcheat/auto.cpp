
// https://en.cppreference.com/w/cpp/language/auto
// https://en.cppreference.com/w/cpp/language/function
// https://en.cppreference.com/w/cpp/language/structured_binding

#include <type_traits>
#include <catch2/catch.hpp>

namespace {

    TEST_CASE("auto variables", "[auto]")
    {
        // when using auto for the type of a variable
        // the variable's type is deduced by the compiler thanks to the right expression's type

        // this variable's type is explicitely: int
        int object_of_explicit_type = 5;

        REQUIRE((std::is_same_v<decltype(object_of_explicit_type), int>));
        REQUIRE(object_of_explicit_type == 5);

        // this variable's type is deduced as: int
        auto objet_of_deduced_type = object_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(objet_of_deduced_type), int>));
        REQUIRE(objet_of_deduced_type == 5);

        // this variable's type is explicitely: int&
        int& reference_of_explicit_type = object_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(reference_of_explicit_type), int&>));
        REQUIRE(reference_of_explicit_type == 5);

        // this variable's type is deduced as: int (not int&)
        auto objet_of_type_deduced_from_a_reference = reference_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(objet_of_type_deduced_from_a_reference), int>));
        REQUIRE(objet_of_type_deduced_from_a_reference == 5);

        // this variable's type is deduced as: int&
        auto& reference_of_type_deduced_from_another_reference = reference_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(reference_of_type_deduced_from_another_reference), int&>));
        REQUIRE(reference_of_type_deduced_from_another_reference == 5);

        // this variable's type is deduced as: int&
        auto& reference_of_type_deduced_from_an_object = object_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(reference_of_type_deduced_from_an_object), int&>));
        REQUIRE(reference_of_type_deduced_from_an_object == 5);

        // this variable's type is deduced as: const int&
        const auto& reference_of_const_type_deduced_from_an_object = object_of_explicit_type;

        REQUIRE((std::is_same_v<decltype(reference_of_const_type_deduced_from_an_object), const int&>));
        REQUIRE(reference_of_const_type_deduced_from_an_object == 5);

        // this variable's type is deduced as: std::pair<bool, double>
        auto pair_of_bool_double = std::pair{true, 12.34};

        REQUIRE((std::is_same_v<decltype(pair_of_bool_double), std::pair<bool, double>>));
        REQUIRE((pair_of_bool_double == std::pair{true, 12.34}));

        // these variables' types are deduced respectively as: bool and double
        auto [b, d] = pair_of_bool_double;

        REQUIRE((std::is_same_v<decltype(b), bool>));
        REQUIRE(b == true);
        REQUIRE((std::is_same_v<decltype(d), double>));
        REQUIRE(d == Approx(12.34));
    }

    // The return-type of this function is int, because it returns the type of the expression 'i + 1'
    auto func(int i)
    {
        return i + 1;
    }

    TEST_CASE("function returning auto", "[auto]")
    {
        const auto result = func(41);

        REQUIRE(result == 42);
    }

} // namespace
