
#pragma once

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace bac {

    // ask the user to enter a string
    std::string ask_string(std::istream& in);

    // ask the user to enter something of a specified type wrapped in an optional
    // if the user provides something that is not the representation of a valid value
    // then return std::nullopt
    template <typename T>
    std::optional<T> ask(std::istream& in)
    {
        const auto string_value = ask_string(in);
        std::istringstream iss(string_value);
        T value{};
        iss >> value;
        if (iss.fail() || iss.peek() != std::stringstream::traits_type::eof())
            return std::nullopt;
        return value;
    }

} // namespace bac
