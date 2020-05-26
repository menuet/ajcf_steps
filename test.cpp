
#include <iostream>
#include "calc.hpp"
#include "calcsuper.hpp"

int main()
{
    std::cout << "Hello\n";
    std::cout << add(2, 3) << "\n";
    std::cout << "double de 5 = " << calcul_double(5) << "\n";
    print_current_date();
    return 0;
}
