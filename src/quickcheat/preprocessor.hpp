
// https://en.wikipedia.org/wiki/Include_guard
// https://en.cppreference.com/w/cpp/preprocessor/impl

// first possible simple option: use this directive at the beginning of your header file:
#pragma once

// else: other option: use an include guard, as follows:

// The preprocessor checks if MY_GUARD_FOR_THE_HEADER_FILE_PREPROCESSOR_HPP is already defined
// If it is not, then it looks at the code after the #if, else it ignores everything until the #endif
#if !defined(MY_GUARD_FOR_THE_HEADER_FILE_PREPROCESSOR_HPP)

// here, we define the guard
// so that next time we include this same file
// the compiler skips everything until the #endif
#define MY_GUARD_FOR_THE_HEADER_FILE_PREPROCESSOR_HPP

class ClasseDefinedOnlyOnceEvenIfWeIncludeTheHeaderFileSeveralTimes
{
};

#endif
