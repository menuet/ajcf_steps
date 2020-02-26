
// https://en.cppreference.com/w/cpp/preprocessor
// https://en.cppreference.com/w/cpp/preprocessor/include
// https://en.cppreference.com/w/cpp/preprocessor/replace
// https://en.cppreference.com/w/cpp/preprocessor/conditional

// include a standard header
#include <iostream>

// include a third-party library header
#include <fmt/format.h>

// include a header in the same directory as the source file
#include "preprocessor.hpp"
// include the same header again
// this is not problematic as long as the header contains include-guards or #pragma once directive
#include "preprocessor.hpp"

// define a constant thanks to the preprocessor (NOT RECOMMENDED)
// the preprocessor only replaces the text "Ma_Constante" by the text "654" everywhere in the source text.
// there is not semantic analysis, no type checking
#define My_Pseudo_Constant_With_Preprocessor 654

// modern equivalent without preprocessor (RECOMMENDED)
// the compiler knows that Ma_Constante is a constant of type int and of value 654
// it will not allow the usage of this contant inside semantically incorrect expressions
constexpr int Ma_Constante_Semantique = 654;

// conditional compilation
#if defined(My_Pseudo_Constant_With_Preprocessor)
// si part of the code is compiled untile the #else
int variable_compilee;
#else
// this part is not compiled because My_Pseudo_Constant_With_Preprocessor is defined
int variable_ignoree;
#endif
