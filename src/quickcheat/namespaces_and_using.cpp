
// https://en.cppreference.com/w/cpp/language/namespace
// https://en.cppreference.com/w/cpp/language/using_declaration
// https://en.cppreference.com/w/cpp/language/type_alias

#include <iostream>

namespace my_namespace {
    // entities in the namespace my_namespace
    class C
    {
    };
} // namespace my_namespace

// nested namespaces
namespace my_namespace { namespace my_sub_namespace {
    // entities in namespace my_namespace::my_sub_namespace
    const double PI = 3.1415;
}} // namespace my_namespace::my_sub_namespace

// Short syntax (C++17)
namespace my_namespace::my_sub_namespace {
    // other entities in namespace my_namespace::my_sub_namespace
    constexpr double PI_DIV_2 = 3.1415 / 2.0;
} // namespace my_namespace::my_sub_namespace

// Use entity defined in another namespace
my_namespace::C obj;

// Namespace alias
namespace my_alias = my_namespace::my_sub_namespace;

void func()
{
    const double PI_div_4 = my_alias::PI_DIV_2 / 2;
    std::cout << "PI/4 = " << PI_div_4 << "\n";
}

// Anonymous namespace: provide internal linkage
namespace {
    int static_global_var; // linkage: internal
}

// Equivalent to:
static int other_static_global_var; // linkage: internal

// Using-Directive (SHOULD BE AVOIDED)
using namespace my_namespace;
C an_obj;

// Using-declaration (PREFERED)
using my_namespace::C;
C another_obj;

// Type alias (PREFERED)
using D = my_namespace::C;
// Equivalent old syntax (AVOID)
typedef my_namespace::C D2;

D c1;
D2 c2; // c1 et c2 have the same type
