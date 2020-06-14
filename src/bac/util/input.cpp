
#include "input.hpp"

namespace bac {

    std::string ask_string(std::istream& in)
    {
        std::string value{};
        std::getline(in, value);
        return value;
    }

} // namespace bac
