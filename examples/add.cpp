#include "flute.hpp"

int main()
{
    using namespace flute;

    using ufixed_t = ufixed<4, 32>;
    using raw_t = ufixed_t::value_type;

    constexpr double d_a = 3.21;
    constexpr double d_b = 1.23;

    constexpr double d_sum = d_a + d_b;

    constexpr ufixed_t a{d_a};
    constexpr ufixed_t b{d_b};


    constexpr auto sum = a + b;
    constexpr auto sum_double = sum.as<double>();

    constexpr auto eps = epsilon<ufixed_t, double>::value;

    constexpr auto lb = sum_double - 2 * eps;
    constexpr auto ub = sum_double + 2 * eps;

    static_assert(lb < d_sum && d_sum < ub);
}