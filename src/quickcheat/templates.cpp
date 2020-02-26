
// https://en.cppreference.com/w/cpp/language/templates
// https://en.cppreference.com/w/cpp/language/template_parameters
// https://en.cppreference.com/w/cpp/language/static_assert
// https://en.cppreference.com/w/cpp/header/type_traits

#include <type_traits>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <sstream>
#include <string>
#include <vector>

namespace {

    // normal function
    std::string get_some_text_from(int i)
    {
        return fmt::format("function get_some_text_from({})", i);
    }

    // primary function template
    template <typename T>
    std::string get_some_text_from(const T& t)
    {
        static_assert(!std::is_class<T>::value);

        return fmt::format("instantiation of primary get_some_text_from<T>(const T& : {})", t);
    }

    // function template specialization when T is std::string
    template <>
    std::string get_some_text_from<std::string>(const std::string& s)
    {
        return fmt::format("instantiation of specialized get_some_text_from<std::string>(const std::string& : {})", s);
    }

    // another primary function template overloaded with different function parameters
    template <typename T>
    std::string get_some_text_from(const T* t)
    {
        static_assert(!std::is_class<T>::value);

        return fmt::format("instantiation of primary get_some_text_from<T>(const T* : {})", t);
    }

    TEST_CASE("function templates", "[functions][templates]")
    {
        // call the function
        REQUIRE(get_some_text_from(123) == "function get_some_text_from(123)");

        // explicitely call the function template instantiation for type int
        REQUIRE(get_some_text_from<int>(123) == "instantiation of primary get_some_text_from<T>(const T& : 123)");

        // implicitely call the function template instantiation for type double
        // (deduced by the compiler from the argument type)
        REQUIRE(get_some_text_from(12.45) == "instantiation of primary get_some_text_from<T>(const T& : 12.45)");

        // implicitely call the function template instantiation for type char[6]
        // (deduced by the compiler from the argument type)
        REQUIRE(get_some_text_from("hello") == "instantiation of primary get_some_text_from<T>(const T* : hello)");

        // implicitely call the function template instantiation for type std::string
        // (deduced by the compiler from the argument type)
        // this calls the specialization
        REQUIRE(get_some_text_from(std::string{"hello"}) ==
                "instantiation of specialized get_some_text_from<std::string>(const std::string& : hello)");

#if 0
        class C {};
        C c;
        REQUIRE(get_some_text_from(c) == ""); // compilation error because of static_assert : error C2607: static assertion failed
#endif
    }

    // primary class template
    template <typename T, typename U>
    struct ClassTemplate
    {
        std::string get_some_text_from()
        {
            return "ClassTemplate<T, U>::get_some_text_from()";
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
        std::string get_some_text_from()
        {
            return "ClassTemplate<X, X>::get_some_text_from()";
        }
    };

    // full specialization of class template for case T = int and U = bool
    template <>
    struct ClassTemplate<int, bool>
    {
        std::string get_some_text_from()
        {
            return "ClassTemplate<int, bool>::get_some_text_from()";
        }
    };

    // explicit instantation of class template, in order to force code generation
    template struct ClassTemplate<int, float>;

    TEST_CASE("class templates", "[classes][templates]")
    {
        // create an object which type is instantiated thanks to the primary template
        ClassTemplate<std::string, double> t1;

        REQUIRE(t1.get_some_text_from() == "ClassTemplate<T, U>::get_some_text_from()");

        // create an object which type is instantiated
        // thanks to the partial specialization where T == U
        ClassTemplate<bool, bool> t2;

        REQUIRE(t2.get_some_text_from() == "ClassTemplate<X, X>::get_some_text_from()");

        // create an object which type is instantiated
        // thanks to the full specialization where T == int and U == bool
        ClassTemplate<int, bool> t3;

        REQUIRE(t3.get_some_text_from() == "ClassTemplate<int, bool>::get_some_text_from()");

        // call the member function member_function_template<char>
        REQUIRE(t1.member_function_template('a') == "ClassTemplate<T, U>::member_function_template<V>(a)");
    }

    template <unsigned int U>
    struct ClassTemplateWithNonTypeTemplateParameter
    {
        char some_array[U]{};
    };

    template <auto Val> // C++17
    struct ClassTemplateWithAutoNonTypeTemplateParameter
    {
        static constexpr auto value{Val};
    };

    template <template <typename> class T>
    struct ClassTemplateWithTemplateTemplateParameter
    {
        T<char> some_container_of_char{};
    };

    template <typename T>
    struct ClassTemplateAsTemplateParameter
    {
    };

    TEST_CASE("other kinds of tempate parameters", "[templates]")
    {
        ClassTemplateWithNonTypeTemplateParameter<123U> an_obj;
        REQUIRE(sizeof(an_obj.some_array) == 123U * sizeof(char));

        ClassTemplateWithAutoNonTypeTemplateParameter<'A'> another_obj;
        REQUIRE(another_obj.value == 'A');

        ClassTemplateWithTemplateTemplateParameter<ClassTemplateAsTemplateParameter> yet_another_obj;
        REQUIRE(
            std::is_same_v<decltype(yet_another_obj.some_container_of_char), ClassTemplateAsTemplateParameter<char>>);
    }

    // Since C++14, one can define variable templates
    template <typename T>
    auto is_enum = std::is_enum<T>::value;

    enum Enum
    {
        one,
        two
    };

    TEST_CASE("variable templates", "[templates]")
    {
        REQUIRE(is_enum<Enum>);

        is_enum<Enum> = false;

        REQUIRE(!is_enum<Enum>);

        REQUIRE(!is_enum<int>);
    }

    void print_impl([[maybe_unused]] std::ostream& out, [[maybe_unused]] std::size_t index)
    {
    }

    template <typename Param>
    void print_impl(std::ostream& out, std::size_t index, Param param)
    {
        out << "[" << index << "]=" << param;
    }

    template <typename FirstParam, typename SecondParam, typename... Params>
    void print_impl(std::ostream& out, std::size_t index, FirstParam first, SecondParam second, Params... params)
    {
        print_impl(out, index, first);
        out << ", ";
        print_impl(out, index + 1, second, params...);
    }

    template <typename... Params>
    void count_and_print(std::ostream& out, Params... params)
    {
        out << "There are " << sizeof...(params) << " parameters: ";
        print_impl(out, 0, params...);
    }

    TEST_CASE("variadic templates", "[templates]")
    {
        {
            std::stringstream out{};
            count_and_print(out);
            REQUIRE(out.str() == "There are 0 parameters: ");
        }
        {
            std::stringstream out{};
            count_and_print(out, "one");
            REQUIRE(out.str() == "There are 1 parameters: [0]=one");
        }
        {
            std::stringstream out{};
            count_and_print(out, "one", std::string{"two"});
            REQUIRE(out.str() == "There are 2 parameters: [0]=one, [1]=two");
        }
        {
            std::stringstream out{};
            count_and_print(out, 12, 23.45, 'A', "hello", true);
            REQUIRE(out.str() == "There are 5 parameters: [0]=12, [1]=23.45, [2]=A, [3]=hello, [4]=1");
        }
    }

    template <typename... Params>
    void count_and_print_with_fold_expression(std::ostream& out, Params... params)
    {
        out << "There are " << sizeof...(params) << " parameters: ";
        ((out << params << ", "), ...);
    }

    template <typename... Params>
    auto sum(Params... params)
    {
        return (params + ...);
    }

    template <typename Init, typename... Params>
    auto sum_with_init(Init init, Params... params)
    {
        return (params + ... + init);
    }

    TEST_CASE("fold expressions", "[templates]")
    {
        std::stringstream out{};
        count_and_print_with_fold_expression(out, 12, 23.45, 'A', "hello", true);
        REQUIRE(out.str() == "There are 5 parameters: 12, 23.45, A, hello, 1, ");

        REQUIRE(sum(12) == 12);
        REQUIRE(sum(21, 87, 32, 1) == 141);

        REQUIRE(sum_with_init(23, 12) == 35);
        REQUIRE(sum_with_init(1, 21, 87, 32, 1) == 142);
    }

} // namespace
