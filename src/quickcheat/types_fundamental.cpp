
// https://en.cppreference.com/w/cpp/language/types
// https://en.cppreference.com/w/cpp/language/bool_literal
// https://en.cppreference.com/w/cpp/language/integer_literal
// https://en.cppreference.com/w/cpp/language/character_literal
// https://en.cppreference.com/w/cpp/language/floating_literal
// https://en.cppreference.com/w/cpp/language/string_literal

// note:
// there are lot's of fundamental types,
// notably all those related to integers
//   - unsigned / signed
//   - of several sizes : short / long / long long
// We only show some of the most used ones.

namespace {

    // boolean
    // 2 possible values : true or false
    bool b = true;

    // character
    // note : each character has an associated numerical value
    constexpr char c1 = 'A';
    constexpr char c2 = 65; // c1 et c2 have the same value
    static_assert(c1 == c2);

    // C-style string, implicitely terminated by '\0'
    char string[] = "Hello";

    // signed integer of size of the machine architecture
    int integer = 1234;

    // real of 64 bits
    double real = 1234.5678;

} // namespace
