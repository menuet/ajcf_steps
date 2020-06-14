
#pragma once

#include "board.hpp"

namespace bac {

    struct Options;

    class Codemaker
    {
    public:
        Feedback compare_attempt_with_secret_code(std::ostream& out, const Code& attempt);

        Code choose_secret_code(std::ostream& out, const Options& options);

    private:
        Code m_secret_code{};
    };

} // namespace bac
