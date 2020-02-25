
#include <iostream>
#include <string>

int main()
{
    std::cout << "Please create your account.\n";

    std::cout << "What is your name ? ";
    std::string name;
    std::cin >> name;

    std::cout << "What is your password ? ";
    std::string password;
    std::cin >> password;

    std::cout << "What is your gender (M/F) ? ";
    char gender;
    std::cin >> gender;

    std::cout << "What is your age ? ";
    int age;
    std::cin >> age;

    std::cout << "What is your size ? ";
    double size;
    std::cin >> age;

    std::cout << "Thanks. Your account was successfully created:\n"
              << "Name: " << name << "\n"
              << "Password length: " << password.length() << "\n"
              << "Gender: " << gender << "\n"
              << "Age: " << age << " years\n"
              << "Size: " << size << " m\n";

    return 0;
}
