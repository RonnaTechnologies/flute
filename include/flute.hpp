# pragma once

#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace flute
{
    namespace detail
    {
        template <std::size_t>
        struct unsigned_raw { using type = void; };

        template <std::size_t n_bits> requires (n_bits <= 8)
        struct unsigned_raw<n_bits> { using type = std::uint8_t; };

        template <std::size_t n_bits> requires (n_bits > 8 && n_bits <= 16)
        struct unsigned_raw<n_bits> { using type = std::uint16_t; };

        template <std::size_t n_bits> requires (n_bits > 16 && n_bits <= 32)
        struct unsigned_raw<n_bits> { using type = std::uint32_t; };

        template <std::size_t n_bits> requires (n_bits > 32 && n_bits <= 64
                                                && std::numeric_limits<std::uintmax_t>::digits > 32)
        struct unsigned_raw<n_bits> { using type = std::uint64_t; };

        template <std::size_t n_bits> requires (n_bits > 64)
        struct unsigned_raw<n_bits> { using type = void; };
    }

    template <std::size_t I, std::size_t F>
    class ufixed
    {
        using T = detail::unsigned_raw<I + F>::type;

    public:

        using value_type = T;

        constexpr ufixed() = default;

        template <typename FloatingPoint_t>
        requires std::is_floating_point_v<FloatingPoint_t>
        constexpr ufixed(FloatingPoint_t f) 
        : raw{static_cast<T>(f * static_cast<FloatingPoint_t>(T{1} << F))}
        {

        }

        template <typename Integer_t>
        requires std::is_integral_v<Integer_t>
        constexpr ufixed(Integer_t i)
        : raw{static_cast<T>(i << F)}
        {

        }

        template <typename Dest_t>
        requires (std::is_floating_point_v<Dest_t> && sizeof(Dest_t) >= sizeof(T))
        constexpr Dest_t as() const noexcept
        {
            return (static_cast<Dest_t>(raw / static_cast<Dest_t>(T{1} << F)));
        }

        template <typename Dest_t>
        requires std::is_integral_v<Dest_t>
        constexpr Dest_t as() const noexcept
        {
            return raw >> F;
        }

        template <typename Raw_t>
        requires std::is_convertible_v<Raw_t, T>
        static constexpr auto from_raw(Raw_t&& r)
        {
            ufixed<I, F> f;
            f.raw = r;
            return f;
        }

        constexpr auto data() const noexcept { return raw; }

        friend constexpr auto operator + (ufixed<I, F> a, ufixed<I, F> b)
        {
            ufixed<I, F> uf;
            uf.raw = a.raw + b.data();

            return uf;
        }

        friend constexpr auto operator - (ufixed<I, F> a, ufixed<I, F> b)
        {
            ufixed<I, F> uf;
            uf.raw = a.raw - b.data();

            return uf;
        }

        template <typename Int>
        requires std::is_integral_v<Int>
        friend constexpr auto operator * (Int a, ufixed<I, F> b)
        {
            using overflow_t = detail::unsigned_raw<F + I + std::numeric_limits<Int>::digits>::type;

            return ufixed<I, F>::from_raw(static_cast<overflow_t>(a * b.data()));
        }

    private:

        T raw{};
    };

    template <typename, typename>
    struct epsilon;

    template <std::size_t I, std::size_t F, typename Dest_t>
    requires (std::is_floating_point_v<Dest_t> && sizeof(Dest_t) <= sizeof(std::uintmax_t))
    struct epsilon<ufixed<I, F>, Dest_t>
    {
        static constexpr Dest_t value = 1. / (std::uintmax_t{1} << F);
    };

} // namespace flute
