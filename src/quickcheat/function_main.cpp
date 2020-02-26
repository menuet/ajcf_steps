
// https://en.cppreference.com/w/cpp/language/main_function

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <iostream>
#include <utility>

// program's main function
// There must be one and only one function named main
// it returns a value of type int
// it takes, optionally, 2 parameters:
//    - the first one contains the number of command-line's arguments
//      (there is always at least one implicit argument: the program's name)
//    - the second one contains an array of all the command-line's arguments
int main(int argc, char* argv[])
{
    std::cout << "---------------------\n";

    std::cout << "List of command-line's arguments:\n";

    for (int arg_index = 0; arg_index < argc; ++arg_index)
    {
        const char* const argument = argv[arg_index];

        std::cout << "  argument #" << arg_index << " = \"" << argument << "\"\n";
    }

    std::cout << "---------------------\n";

    // delegate the real work to the Catch Framework
    const int result = Catch::Session().run(argc, argv);

    // convention: usually, one return 0 to say "everything went fine"
    return result;
}
