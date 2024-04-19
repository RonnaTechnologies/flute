#include "flute.hpp"

int main()
{
    using namespace flute;

    using ufixed_t = ufixed<4, 12>;
    using raw_t = ufixed_t::value_type;

    constexpr raw_t two{2};
    constexpr ufixed_t pi{3.14};

    constexpr auto two_times_pi = two * pi;
    constexpr auto two_times_pi_double = two_times_pi.as<double>();
}