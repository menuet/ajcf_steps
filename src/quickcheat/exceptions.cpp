
#include <catch2/catch.hpp>
#include <tl/expected.hpp>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace {

    class MyException
    {
    };

    class MyExceptionDerivedFromStd : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    std::string function_which_throws_an_exception_sometime(int choice, const char* text)
    {
        switch (choice)
        {
        case 1:
            throw 1234;
        case 2:
            throw std::runtime_error("run time error");
        case 3:
            throw MyException();
        case 4:
            throw MyExceptionDerivedFromStd("boum");
        }

        return std::string(" Everything is ok, we handled this text: ") + text;
    }

    void function_which_promises_not_to_throw_any_exception(char* text, unsigned length, char fill_with) noexcept
    {
        for (unsigned i = 0; i < length; ++i)
            text[i] = fill_with;
        text[length] = 0;
    }

    namespace exceptions {

        void function_which_propagates_exceptions_without_leaks(std::stringstream& result, int choice_1, int choice_2)
        {
            // can throw std::bad_alloc
            char* text1 = new char[20 + 1];

            function_which_promises_not_to_throw_any_exception(text1, 20, static_cast<char>('0' + choice_1));
            result << text1;

            try
            {
                // can throw some user-defined exceptions
                result << function_which_throws_an_exception_sometime(choice_1, text1);

                result << " Step 1 ok ";

                // can throw std::bad_alloc
                char* text2 = new char[20 + 1];

                function_which_promises_not_to_throw_any_exception(text2, 10, static_cast<char>('0' + choice_2));
                result << text2;

                try
                {
                    // can throw some user-defined exceptions
                    result << function_which_throws_an_exception_sometime(choice_2, text2);

                    result << " Step 2 ok ";

                    delete[] text2;
                    delete[] text1;
                }
                catch (...) // catch all possible exceptions to do some clean up
                {
                    delete[] text2;
                    throw; // propagate the exception to upper catch levels
                }
            }
            catch (...) // catch all possible exceptions to do some clean up
            {
                delete[] text1;
                throw; // propagate the exception to upper catch levels
            }
        }

        std::string function_which_catches_exceptions_and_handles_them(int choice_1, int choice_2)
        {
            std::stringstream result;

            try
            {
                function_which_propagates_exceptions_without_leaks(result, choice_1, choice_2);
            }
            catch (int i)
            {
                result << " Exception of type int, of value : " << i;
            }
            catch (MyException&)
            {
                result << " Exception of type MyException";
            }
            catch (MyExceptionDerivedFromStd& ex)
            {
                result << " Exception of type MyExceptionDerivedFromStd, with message: " << ex.what();
            }
            catch (std::runtime_error& ex)
            {
                result << " Exception of type std::runtime_error, with message: " << ex.what();
            }
            catch (std::exception& ex)
            {
                result << " Exception of type std::exception, with message: " << ex.what();
            }
            catch (...)
            {
                result << " Exception of unknown type";
            }

            return result.str();
        }

        TEST_CASE("exceptions", "[exceptions]")
        {
            REQUIRE(function_which_catches_exceptions_and_handles_them(1, 2) ==
                    "11111111111111111111 Exception of type int, of value : 1234");

            REQUIRE(function_which_catches_exceptions_and_handles_them(2, 3) ==
                    "22222222222222222222 Exception of type std::runtime_error, with message: run time error");

            REQUIRE(function_which_catches_exceptions_and_handles_them(3, 4) ==
                    "33333333333333333333 Exception of type MyException");

            REQUIRE(function_which_catches_exceptions_and_handles_them(4, 5) ==
                    "44444444444444444444 Exception of type MyExceptionDerivedFromStd, with message: boum");

            REQUIRE(function_which_catches_exceptions_and_handles_them(5, 6) ==
                    "55555555555555555555 Everything is ok, we handled this text: 55555555555555555555 Step 1 ok "
                    "6666666666 Everything is ok, we handled this text: 6666666666 Step 2 ok ");
        }

    } // namespace exceptions

    namespace expected {

        // note : by using a RAII types (like std::unique_ptr, std::string, std::vector, ...),
        // on can avoid most of the try/catch ceremony
        std::string function_which_returns_a_string_or_propagates_exceptions_without_leaks(int choice_1, int choice_2)
        {
            std::stringstream result;

            // can throw std::bad_alloc
            auto text1 = std::make_unique<char[]>(20 + 1);

            function_which_promises_not_to_throw_any_exception(text1.get(), 20, static_cast<char>('0' + choice_1));
            result << text1.get();

            // can throw some user-defined exceptions
            result << function_which_throws_an_exception_sometime(choice_1, text1.get());

            result << " Step 1 ok ";

            // can throw std::bad_alloc
            auto text2 = std::vector<char>(20 + 1);

            function_which_promises_not_to_throw_any_exception(text2.data(), 10, static_cast<char>('0' + choice_2));
            result << text2.data();

            // can throw some user-defined exceptions
            result << function_which_throws_an_exception_sometime(choice_2, text2.data());

            result << " Step 2 ok ";

            return result.str();
        }

        using StringOrError = tl::expected<std::string, std::exception_ptr>;
        using IntOrError = tl::expected<int, std::exception_ptr>;
        using Error = tl::unexpected<std::exception_ptr>;
        using DoubleOrErrorText = tl::expected<double, std::string>;

        StringOrError function_which_catches_exceptions_and_returns_a_string_or_error(int choice_1, int choice_2)
        {
            try
            {
                if (choice_1 < 0)
                    return "-42";
                return function_which_returns_a_string_or_propagates_exceptions_without_leaks(choice_1, choice_2);
            }
            catch (...)
            {
                return Error{std::current_exception()};
            }
        }

        IntOrError function_which_takes_a_string_and_returns_an_int_or_error(const std::string& text) noexcept
        {
            try
            {
                std::istringstream iss(text);
                int value{};
                iss >> value;
                if (iss.fail() || iss.peek() != std::istringstream::traits_type::eof())
                    throw std::domain_error{"string does not contain a number"};
                return value;
            }
            catch (const std::exception&)
            {
                return Error{std::current_exception()};
            }
        }

        double function_which_takes_an_int_and_returns_a_double(int value) noexcept
        {
            return static_cast<double>(value) / 5;
        }

        std::string convert_exception_to_text(std::exception_ptr ep)
        {
            try
            {
                if (ep)
                    std::rethrow_exception(ep);
            }
            catch (int i)
            {
                return std::to_string(i);
            }
            catch (MyException&)
            {
                return std::string{"MyException"};
            }
            catch (const std::exception& e)
            {
                return std::string{"std::exception:"} + e.what();
            }
            catch (...)
            {
                return std::string{"Exception of unknown type"};
            }
            return std::string{};
        }

        DoubleOrErrorText function_which_chains_all_the_previous_functions(int choice_1, int choice_2)
        {
            return function_which_catches_exceptions_and_returns_a_string_or_error(choice_1, choice_2)
                .and_then(function_which_takes_a_string_and_returns_an_int_or_error)
                .map(function_which_takes_an_int_and_returns_a_double)
                .map_error(convert_exception_to_text);
        }

        TEST_CASE("expected", "[exceptions]")
        {
            {
                const auto result = function_which_chains_all_the_previous_functions(1, 2);
                REQUIRE(!result);
                REQUIRE(result.error() == "1234");
            }

            {
                const auto result = function_which_chains_all_the_previous_functions(2, 3);
                REQUIRE(!result);
                REQUIRE(result.error() == "std::exception:run time error");
            }

            {
                const auto result = function_which_chains_all_the_previous_functions(3, 4);
                REQUIRE(!result);
                REQUIRE(result.error() == "MyException");
            }

            {
                const auto result = function_which_chains_all_the_previous_functions(4, 5);
                REQUIRE(!result);
                REQUIRE(result.error() == "std::exception:boum");
            }

            {
                const auto result = function_which_chains_all_the_previous_functions(5, 6);
                REQUIRE(!result);
                REQUIRE(result.error() == "std::exception:string does not contain a number");
            }

            {
                const auto result = function_which_chains_all_the_previous_functions(-1, 7);
                REQUIRE(result);
                REQUIRE(result.value() == Approx{-8.4});
            }
        }

    } // namespace expected

} // namespace
