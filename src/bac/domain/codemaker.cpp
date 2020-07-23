
#include "codemaker.hpp"
#include "logic.hpp"
#include <bac/util/random.hpp>
#include <iostream>

namespace bac {

    Feedback Codemaker::compare_attempt_with_secret_code(std::ostream& out, const Code& attempt)
    {
        out << "[Codemaker] Comparing codebreaker's attempt with secret code...\n";
        return bac::compare_attempt_with_secret_code(attempt, m_secret_code);
    }

    Code Codemaker::choose_secret_code(std::ostream& out, const Options& options)
    {
        out << "[Codemaker] Choosing secret code...\n";
        m_secret_code = generate_random_secret_code(options);
        return m_secret_code;
    }

} // namespace bac
