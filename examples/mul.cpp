#include "flute.hpp"

int main()
{
    using namespace flute;

    using ufixed_t = ufixed<4, 12>;
    using raw_t = ufixed_t::value_type;

    constexpr double pi_double = 3.14;

    constexpr raw_t two{2};
    constexpr ufixed_t pi{pi_double};

    constexpr auto two_times_pi = two * pi;
    constexpr auto two_times_pi_double = two_times_pi.as<double>();

    constexpr auto eps = epsilon<ufixed_t, double>::value;

    constexpr auto lb = 2 * pi_double - 2 * eps;
    constexpr auto ub = 2 * pi_double + 2 * eps;

    static_assert(lb < two_times_pi_double && two_times_pi_double < ub);
}