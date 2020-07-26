
#include "codemaker.hpp"
#include "logic.hpp"
#include <bac/util/logs.hpp>
#include <bac/util/random.hpp>
#include <iostream>

namespace bac {

    Feedback Codemaker::compare_attempt_with_secret_code(std::ostream& out, const Code& attempt)
    {
        BAC_LOG(out, INFO, CODEMAKER, "[Codemaker] Comparing codebreaker's attempt with secret code...");
        return bac::compare_attempt_with_secret_code(attempt, m_secret_code);
    }

    Code Codemaker::choose_secret_code(std::ostream& out, const Options& options)
    {
        BAC_LOG(out, INFO, CODEMAKER, "Choosing secret code...");
        m_secret_code = generate_random_secret_code(options);
        BAC_LOG(out, INFO, CODEMAKER, "Choosen secret code: " << m_secret_code.value);
        return m_secret_code;
    }

} // namespace bac
