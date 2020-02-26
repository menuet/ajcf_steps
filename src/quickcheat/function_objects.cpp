
// https://en.cppreference.com/w/cpp/language/lambda

#include <catch2/catch.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace {

    namespace example_function_pointer {

        int add(int x, int y)
        {
            return x + y;
        }

        int sub(int x, int y)
        {
            return x - y;
        }

        int mul(int x, int y)
        {
            return x * y;
        }

        int div(int x, int y)
        {
            return x / y;
        }

        using FunctionReturningIntAndTakingTwoInts = int(int, int);

        int compute(int x, int y, FunctionReturningIntAndTakingTwoInts* operation)
        {
            const auto result = operation(x, y);
            return result;
        }

        bool is_operator(char op)
        {
            return op == '+' || op == '-' || op == '*' || op == '/' || op == '%';
        }

        std::map<char, FunctionReturningIntAndTakingTwoInts*> possible_operations = {
            {'+', &add},
            {'-', &sub},
            {'*', &mul},
            {'/', &div},
        };

        int demo_function_pointers(std::string expression)
        {
            const auto iter_operator = std::find_if(expression.begin(), expression.end(), &is_operator);
            if (iter_operator == expression.end())
                throw std::invalid_argument("No arithmetic operator in expression");

            const char operator_char = *iter_operator;
            const auto iter_operation = possible_operations.find(operator_char);
            if (iter_operation == possible_operations.end())
                throw std::invalid_argument("Operation not supported");
            const auto operation = iter_operation->second;

            const auto text_first_operand = expression.substr(0, iter_operator - expression.begin());
            const auto first_operand = std::stoi(text_first_operand);

            const auto text_second_operand = expression.substr(iter_operator - expression.begin() + 1);
            const auto second_operand = std::stoi(text_second_operand);

            return compute(first_operand, second_operand, operation);
        }

    } // namespace example_function_pointer

    namespace function_objects {

        class Operation
        {
        public:
            Operation(char operator_char) : m_operator_char(operator_char)
            {
            }

            int operator()(int x, int y) const
            {
                switch (m_operator_char)
                {
                case '+':
                    return x + y;
                case '-':
                    return x - y;
                case '*':
                    return x * y;
                case '/':
                    return x / y;
                }
                throw std::runtime_error("operation non supportee !!!");
            }

        private:
            char m_operator_char;
        };

        struct IsOperator
        {
            bool operator()(char operator_char) const
            {
                return operator_char == '+' || operator_char == '-' || operator_char == '*' || operator_char == '/';
            }
        };

        int compute(int x, int y, Operation operation)
        {
            const auto result = operation(x, y);
            return result;
        }

        int demo_function_objects(std::string expression)
        {
            const auto iter_operator = std::find_if(expression.begin(), expression.end(), IsOperator{});
            if (iter_operator == expression.end())
                throw std::invalid_argument("No arithmetic operator in expression");

            const auto operator_char = *iter_operator;

            const auto text_first_operand = expression.substr(0, iter_operator - expression.begin());
            const auto first_operand = std::stoi(text_first_operand);

            const auto text_second_operand = expression.substr(iter_operator - expression.begin() + 1);
            const auto second_operand = std::stoi(text_second_operand);

            Operation operation{operator_char};

            return compute(first_operand, second_operand, operation);
        }

    } // namespace function_objects

    namespace lambdas {

        int compute(int x, int y, std::function<int(int, int)> operation)
        {
            const auto result = operation(x, y);
            return result;
        }

        int demo_lambdas(std::string expression)
        {
            const auto iter_operator = std::find_if(expression.begin(), expression.end(), [](const auto operator_char) {
                return operator_char == '+' || operator_char == '-' || operator_char == '*' || operator_char == '/';
            });

            if (iter_operator == expression.end())
                throw std::invalid_argument("No arithmetic operator in expression");

            const auto operator_char = *iter_operator;

            const auto text_first_operand = expression.substr(0, iter_operator - expression.begin());
            const auto first_operand = std::stoi(text_first_operand);

            const auto text_second_operand = expression.substr(iter_operator - expression.begin() + 1);
            const auto second_operand = std::stoi(text_second_operand);

            return compute(first_operand, second_operand, [operator_char](auto x, auto y) {
                switch (operator_char)
                {
                case '+':
                    return x + y;
                case '-':
                    return x - y;
                case '*':
                    return x * y;
                case '/':
                    return x / y;
                }
                throw std::runtime_error("Unsupported operation");
            });
        }

    } // namespace lambdas

    TEST_CASE("function pointers, function objects, lambda expressions", "[function][pointer][lambda]")
    {
        REQUIRE(example_function_pointer::demo_function_pointers("23+48") == 71);

        REQUIRE(function_objects::demo_function_objects("12*78") == 936);

        REQUIRE(lambdas::demo_lambdas("95/5") == 19);
    }

    namespace more_examples {

        class Something
        {
        public:
            Something(int i, int j) : m_i(i), m_j(j)
            {
            }

            auto get_printer() const
            {
                auto lamb = [this](std::ostream& os) { os << "{" << m_i << "," << m_j << "}"; };

                return lamb;
            }

            auto get_printer_less_dangerous() const
            {
                auto lamb = [i = m_i, j = m_j](std::ostream& os) { os << "{" << i << "," << j << "}"; };

                return lamb;
            }

            auto get_printer_nuclear_bomb() const
            {
                std::string s = "Title";

                auto lamb = [&, this](std::ostream& os) { os << s << " : {" << m_i << "," << m_j << "}"; };

                return lamb;
            }

        private:
            int m_i;
            int m_j;
        };

        void print()
        {
            Something e(1, 2);

            auto printer = e.get_printer();

            printer(std::cout);
        }

        void print_boom()
        {
            std::unique_ptr<Something> e = std::make_unique<Something>(1, 2);

            auto printer = e->get_printer();

            e.reset();

            printer(std::cout); // boom !!!
        }

        void print_ok()
        {
            std::unique_ptr<Something> e = std::make_unique<Something>(1, 2);

            auto printer = e->get_printer_less_dangerous();

            e.reset();

            printer(std::cout);
        }

        void print_mega_boom()
        {
            Something e(1, 2);

            auto printer = e.get_printer_nuclear_bomb();

            printer(std::cout); // boom !!!
        }

    } // namespace more_examples

} // namespace
