
// https://en.cppreference.com/w/cpp/language/cv
// https://en.cppreference.com/w/cpp/language/constexpr

#include <catch2/catch.hpp>
#include <cassert>
#include <string>

namespace {

    // note: the compiler is able to do the computation at compile-time but is not
    // required to do it
    double compute_pi_at_runtime()
    {
        return 62'832.0 / 20'000.0;
    }

    constexpr double compute_pi_at_compiletime_if_possible(int max_iterations)
    {
        // Compute pi with formula : PI = 4/1 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 + ...
        double pi = 0.0;
        for (int i = 0; i < max_iterations; ++i)
        {
            const bool i_is_even = (i % 2) == 0;
            const int numerator = i * 2 + 1;
            if (i_is_even)
                pi += 4.0 / numerator;
            else
                pi -= 4.0 / numerator;
        }
        return pi;
    }

    TEST_CASE("constants", "[const][constexpr]")
    {
        const double CONSTANT_PROBABLY_INITIALIZED_AT_COMPILETIME = 3.141592;

        REQUIRE(CONSTANT_PROBABLY_INITIALIZED_AT_COMPILETIME == Approx(3.141592));

        constexpr double CONSTANT_ALWAYS_INITIALIZED_AT_COMPILETIME = 3.141592;

        REQUIRE(CONSTANT_ALWAYS_INITIALIZED_AT_COMPILETIME == Approx(3.141592));

        // note: the compiler may be able to optimize and compute at compiletime
        const double CONSTANT_INITIALIZED_AT_RUNTIME = compute_pi_at_runtime();

        REQUIRE(CONSTANT_INITIALIZED_AT_RUNTIME == Approx(62'832.0 / 20'000.0));

        constexpr double CONSTANT_ALWAYS_INITIALIZED_AT_COMPILETIME_FROM_A_COMPILETIME_COMPUTATION =
            compute_pi_at_compiletime_if_possible(10);

        REQUIRE(CONSTANT_ALWAYS_INITIALIZED_AT_COMPILETIME_FROM_A_COMPILETIME_COMPUTATION == Approx(3.0418396189));

        int i = 10;
        const double CONSTANT_INITIALIZED_AT_RUNTIME_FROM_A_RUNTIME_COMPUTATION =
            compute_pi_at_compiletime_if_possible(i);

        REQUIRE(CONSTANT_INITIALIZED_AT_RUNTIME_FROM_A_RUNTIME_COMPUTATION == Approx(3.0418396189));
    }

    // this function uses as parameter a copy of the string provided as argument
    bool find_by_copy(std::string s, char c);

    // this function uses as parameter a mutating reference to the string provided as argument
    bool find_by_ref(std::string& s, char c);

    // this function uses as parameter a non-mutating reference to the string provided as argument
    bool find_by_const_ref(const std::string& s, char c);

    TEST_CASE("mutable/immutable parameters", "[const]")
    {
        std::string mutable_string = "sgflkmAmlsdf";

        bool test1 = find_by_copy(mutable_string, 'A');
        // here, we are sure that mutable_string was not modified

        REQUIRE(mutable_string == "sgflkmAmlsdf");
        REQUIRE(test1);

        bool test2 = find_by_ref(mutable_string, 'A');
        // here, maybe mutable_string was modified, it depends on the implementation of find_by_ref

        REQUIRE(mutable_string == "sgflkmAmlsdf-oops"); // indeed mutable_string was modified !
        REQUIRE(test2);

        bool test3 = find_by_const_ref(mutable_string, 'A');
        // here, we are sure that mutable_string was not modified

        REQUIRE(mutable_string == "sgflkmAmlsdf-oops");
        REQUIRE(test3);

        const std::string constant_string = "hello";

        bool test4 = find_by_copy(constant_string, 'A');

        REQUIRE(constant_string == "hello");
        REQUIRE(!test4);

#if 0
        // compilation error C2664: 'bool `anonymous-namespace'::find_by_ref(std::string &,char)': cannot convert argument 1 from 'const std::string' to 'std::string &'
        bool test5 = find_by_ref(constant_string, 'A');
#endif

        bool test6 = find_by_const_ref(constant_string, 'A');

        REQUIRE(constant_string == "hello");
        REQUIRE(!test6);
    }

    bool find_by_copy(std::string s, char c)
    {
        return find_by_const_ref(s, c);
    }

    bool find_by_const_ref(const std::string& s, char c)
    {
        for (char car : s)
        {
            if (car == c)
                return true;
        }
        return false;
    }

    bool find_by_ref(std::string& s, char c)
    {
        bool result = find_by_const_ref(s, c);

        // oops, we modify the string,
        // whereas in a function named find_...,
        // it seems strange to modify the provided argument
        s = s + "-oops";
        return result;
    }

    // Since C++17, if constexpr
    template <typename T>
    auto int_to_string_and_vice_versa(const T& t)
    {
        if constexpr (std::is_integral_v<T>)
        {
            return std::to_string(t);
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return std::stoi(t);
        }
    }

    TEST_CASE("if constexpr", "[constexpr]")
    {
        auto s = int_to_string_and_vice_versa(42);

        REQUIRE(std::is_same_v<decltype(s), std::string>);
        REQUIRE(s == "42");

        auto i = int_to_string_and_vice_versa(std::string{"65"});

        REQUIRE(std::is_same_v<decltype(i), int>);
        REQUIRE(i == 65);
    }

} // namespace
