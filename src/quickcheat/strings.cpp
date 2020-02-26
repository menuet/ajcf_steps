
// https://en.cppreference.com/w/cpp/string
// https://en.cppreference.com/w/cpp/header/string
// https://en.cppreference.com/w/cpp/header/string_view
// https://en.cppreference.com/w/cpp/header/cstring

#include <string_view>
#include <catch2/catch.hpp>
#include <cstring>
#include <string>

namespace {

    TEST_CASE("all kinds of strings", "[strings]")
    {
        // C-style string
        // of size 6 characters because implicitely terminated by '\0'
        char c_string[6] = "Hello";

        REQUIRE(std::strcmp(c_string, "Hello") == 0);

        // get c_string's length
        auto c_string_length = std::strlen(c_string);

        REQUIRE(c_string_length == 5);

        // C++ standard string
        // The string literal "Hello++" is allocated in the static storage.
        // The object cpp_string is allocated in the automatic storage.
        // But class std::string copies the litteral characters into a dyanmic storage.
        std::string cpp_string = "Hello++";

        REQUIRE(cpp_string == "Hello++");

        // get cpp_string's length
        auto cpp_string_length = cpp_string.length();

        REQUIRE(cpp_string_length == 7);

        // get a view on the c_string
        std::string_view view_on_a_string = c_string;

        REQUIRE(view_on_a_string == "Hello");
        REQUIRE(view_on_a_string.length() == 5);

        // rebind the view to cpp_string
        view_on_a_string = cpp_string;

        REQUIRE(view_on_a_string == "Hello++");
        REQUIRE(view_on_a_string.length() == 7);

        // Warning: the observed string's lifetime must exceed the view's lifetime !!!
        {
            std::string cpp_string_of_short_lifetime = "aieaieaie";
            view_on_a_string = cpp_string_of_short_lifetime;

            REQUIRE(view_on_a_string == "aieaieaie");
            REQUIRE(view_on_a_string.length() == 9);
        } // cpp_string_of_short_lifetime is destructed and its associated internal dynamic memory is released

        // Now, view_on_a_string observes a string which storage has been destroyed !!!
        // It is UB if we use it
        // REQUIRE(view_on_a_string == ?);
    }

    unsigned int compute_length_by_searching_for_zero_terminator(const char* pointer_to_beginning_of_c_string)
    {
        const char* end_of_c_string = pointer_to_beginning_of_c_string;

        while (*end_of_c_string != 0)
        {
            ++end_of_c_string;
        }

        return static_cast<unsigned int>(end_of_c_string - pointer_to_beginning_of_c_string);
    }

    TEST_CASE("more C-strings examples", "[strings]")
    {
        char c_string_with_known_max_size[50]{};

        char* c_string_which_max_size_is_not_known_anymore = c_string_with_known_max_size;

        std::strcpy(c_string_which_max_size_is_not_known_anymore, "hello");

        REQUIRE(std::strcmp(c_string_with_known_max_size, "hello") == 0);
        REQUIRE(std::strcmp(c_string_which_max_size_is_not_known_anymore, "hello") == 0);

        const auto std_length = std::strlen(c_string_which_max_size_is_not_known_anymore);

        REQUIRE(std_length == 5);

        const auto computed_length =
            compute_length_by_searching_for_zero_terminator(c_string_which_max_size_is_not_known_anymore);

        REQUIRE(computed_length == 5);
    }

} // namespace
