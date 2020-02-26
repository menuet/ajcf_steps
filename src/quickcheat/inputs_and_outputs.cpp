
// https://en.cppreference.com/w/cpp/io
// https://en.cppreference.com/w/cpp/io/cout
// https://en.cppreference.com/w/cpp/io/cin
// https://en.cppreference.com/w/cpp/io/cerr
// https://en.cppreference.com/w/cpp/header/fstream

#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

    TEST_CASE("to standard output", "[output][!hide]")
    {
        // Write data to the standard output (usually the terminal)
        std::cout << "This is " << 1 << " text with a line break\n";
    }

    TEST_CASE("to standard error output", "[output][!hide]")
    {
        // Write data to the standard error output (usually the terminal too)
        std::cout << "This is " << 1 << " text without a line break";
        std::cerr << " / then another text with lot's of line breaks\n\n\n";
    }

    TEST_CASE("from standard input", "[input][!hide]")
    {
        // Read data from standard input (usually the keyboard)
        int i{};
        std::cin >> i;
        // do something with i
    }

    TEST_CASE("to a file", "[output]")
    {
        // Create file "some_dir/some_file.txt" then write data from it

        std::ofstream ofs("some_dir/some_file.txt");
        if (!ofs)
            return; // cannot be created

        ofs << "One line\n";
        ofs << 1.0 << " other "
            << "line" << '\n';

    } // ofs' destructor automatically closes the file here

    TEST_CASE("from a file", "[input]")
    {
        // Open file "some_dir/some_file.txt" then write data from it

        std::ifstream ifs("mon_rep/mon_fichier.txt");
        if (!ifs)
            return; // cannot be opened

        std::string line1;
        if (!std::getline(ifs, line1))
            return; // Error, ifs' destructor automatically closes the file here

        std::string line2;
        if (!std::getline(ifs, line2))
            return; // Error, ifs' destructor automatically closes the file here

    } // ifs' destructor automatically closes the file here

    TEST_CASE("to a string stream", "[output]")
    {
        std::stringstream ss;

        ss << "some text\n" << 123 << ", text again" << 'X' << 98.7;

        REQUIRE(ss.str() == "some text\n123, text againX98.7");
    }

} // namespace
