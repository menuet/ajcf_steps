
#pragma once

#include <random>

namespace bac {

    // Each time you call this function, it produces a random value
    // between min_value included and max_value included
    template <typename IntegralT>
    IntegralT generate_random_value_between(IntegralT min_value, IntegralT max_value)
    {
        static_assert(std::is_integral_v<IntegralT>);
        using IntegralTypeOrShort = std::conditional_t<sizeof(IntegralT) < sizeof(short), short, IntegralT>;
        thread_local std::random_device rd{};
        thread_local std::mt19937 rg{rd()};
        std::uniform_int_distribution<IntegralTypeOrShort> distrib{min_value, max_value};
        return static_cast<IntegralT>(distrib(rg));
    }

} // namespace bac
