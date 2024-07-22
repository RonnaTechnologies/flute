#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace flute
{

    struct not_found
    {
    };
    struct signed_t
    {
    };
    struct unsigned_t
    {
    };

    namespace detail::types
    {

        // Concepts
        template <typename T>
        concept Sign = std::is_same_v<T, signed_t> || std::is_same_v<T, unsigned_t>;

        // Helpers
        template <typename...>
        struct type_list
        {
        };

        using int_types =
        type_list<std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t>;

        template <auto Predicate, typename NotFound, typename... Ts>
        struct find_if_impl;

        template <auto Predicate, typename NotFound>
        struct find_if_impl<Predicate, NotFound>
        {
            using type = NotFound;
        };

        template <auto Predicate, typename NotFound, typename T, typename... Ts>
        struct find_if_impl<Predicate, NotFound, T, Ts...>
        {
            using type = std::conditional_t<Predicate(T{}), T, typename find_if_impl<Predicate, NotFound, Ts...>::type>;
        };


        template <auto Predicate, typename NotFound, typename List, typename... Ts>
        struct find_if;

        template <template <typename...> typename List, auto Predicate, typename NotFound, typename... Ts>
        struct find_if<Predicate, NotFound, List<Ts...>>
        {
            using type = find_if_impl<Predicate, NotFound, Ts...>::type;
        };


        template <std::size_t n_bits, typename sign_t>
            requires(std::is_same_v<sign_t, signed_t> || std::is_same_v<sign_t, unsigned_t>)
        struct from_bits
        {
            using type = find_if<[]<typename T>(T)
                                 {
                                     return std::numeric_limits<T>::digits >= n_bits &&
                                     std::numeric_limits<T>::is_signed == std::is_same_v<sign_t, signed_t>;
                                 },
                                 not_found,
                                 int_types>::type;
        };
    } // namespace detail::types


    template <std::size_t I, std::size_t F, typename sign_t>
        requires detail::types::Sign<sign_t>
    class fixed
    {
        using T = detail::types::from_bits<I + F, sign_t>::type;

    public:
        using value_type = T;

        constexpr fixed() = default;

        template <typename FloatingPoint_t>
            requires std::is_floating_point_v<FloatingPoint_t>
        constexpr explicit fixed(FloatingPoint_t f)
        : raw{ static_cast<T>(f * static_cast<FloatingPoint_t>(T{ 1 } << F)) }
        {
        }

        template <typename Integer_t>
            requires std::is_integral_v<Integer_t>
        constexpr explicit fixed(Integer_t i) : raw{ static_cast<T>(i << F) }
        {
        }

        template <typename Dest_t>
            requires(std::is_floating_point_v<Dest_t> && sizeof(Dest_t) >= sizeof(T))
        constexpr Dest_t as() const noexcept
        {
            return (static_cast<Dest_t>(raw / static_cast<Dest_t>(T{ 1 } << F)));
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
            fixed<I, F, sign_t> f;
            f.raw = r;
            return f;
        }

        constexpr auto data() const noexcept
        {
            return raw;
        }

        template <typename Int>
            requires std::is_integral_v<Int>
        friend constexpr auto operator+(Int a, fixed<I, F, sign_t> b) noexcept
        {
            return fixed<I, F, sign_t>::from_raw((a << F) + b.data());
        }

        friend constexpr auto operator+(fixed<I, F, sign_t> a, fixed<I, F, sign_t> b) noexcept
        {
            return fixed<I, F, sign_t>::from_raw(a.raw + b.data());
        }

        friend constexpr auto operator-(fixed<I, F, sign_t> a, fixed<I, F, sign_t> b) noexcept
        {
            return fixed<I, F, sign_t>::from_raw(a.raw - b.data());
        }

        template <std::size_t Iout = I, std::size_t Fout = F, std::size_t Ir, std::size_t Fr, typename rsign_t>
            requires std::same_as<sign_t, rsign_t>
        constexpr auto mul(fixed<Ir, Fr, rsign_t> rhs) const noexcept
        {
            using overflow_t = detail::types::from_bits<F + Fr + I + Ir, sign_t>::type;
            return fixed<Iout, Fout, sign_t>::from_raw(static_cast<overflow_t>(raw * rhs.data()) >> Fout);
        }

        template <std::size_t Ir, std::size_t Fr, typename rsign_t>
            requires std::same_as<sign_t, rsign_t>
        friend constexpr auto operator*(fixed<I, F, sign_t> a, fixed<Ir, Fr, rsign_t> b)
        {
            return a.mul(b);
        }

        template <typename Int>
            requires std::is_integral_v<Int>
        friend constexpr auto operator*(Int a, fixed<I, F, sign_t> b) noexcept
        {
            using overflow_t = detail::types::from_bits<F + I + std::numeric_limits<Int>::digits, sign_t>::type;

            return fixed<I, F, sign_t>::from_raw(static_cast<overflow_t>(a * b.data()));
        }

        template <typename Int>
            requires std::is_integral_v<Int>
        friend constexpr auto operator*(fixed<I, F, sign_t> a, Int b) noexcept
        {
            return b * a;
        }

    private:
        T raw{};
    };


    template <std::size_t I, std::size_t F>
    using ufixed = fixed<I, F, unsigned_t>;

    template <std::size_t I, std::size_t F>
    using sfixed = fixed<I, F, signed_t>;


    template <typename, typename>
    struct epsilon;

    template <std::size_t I, std::size_t F, typename sign_t, typename Dest_t>
        requires(std::is_floating_point_v<Dest_t> && sizeof(Dest_t) <= sizeof(std::uintmax_t))
    struct epsilon<fixed<I, F, sign_t>, Dest_t>
    {
        static constexpr Dest_t value = 1. / (std::uintmax_t{ 1 } << F);
    };


    template <typename T, std::size_t N>
    class lut
    {

    public:
        using array_type = std::array<T, N>;
        using value_type = T;

        template <typename Callable>
            requires requires(Callable&& f) { f(typename array_type::size_type{ 0 }); }
        static constexpr lut<T, N> generate(Callable&& f)
        {
            return [](Callable&& f)
            {
                using size_type = array_type::size_type;
                array_type output;
                for (size_type i = 0; i < N; ++i)
                {
                    output[i] = f(i);
                }
                return lut<T, N>{ output };
            }(f);
        }

        constexpr T at(std::size_t i) const noexcept
        {
            return data[i];
        }

        constexpr T& at(std::size_t i) noexcept
        {
            return data[i];
        }

        template <std::size_t I, std::size_t F, typename sign_t>
        constexpr auto at(const fixed<I, F, sign_t>& x) const noexcept
        {
            using fixed_type = fixed<I, F, sign_t>;
            using size_type = array_type::size_type;
            const auto xa = x.template as<size_type>();
            const auto ya = data[xa];
            const auto yb = data[xa + 1];
            return ya + (yb - ya) * (x - fixed_type{ xa });
        }

        static constexpr lut<T, N> make(const std::array<T, N>& input)
        {
            return lut<T, N>{ input };
        }

    private:
        constexpr explicit lut(std::array<T, N>&& arr) : data{ arr }
        {
        }

        constexpr explicit lut(const std::array<T, N>& arr) : data{ arr }
        {
        }

        array_type data;
    };

    template <typename, std::size_t>
    class flut;

    template <std::size_t I, std::size_t F, typename sign_t, std::size_t N>
    class flut<fixed<I, F, sign_t>, N>
    {
        using T = fixed<I, F, sign_t>::value_type;

    public:
        using fixed_type = fixed<I, F, sign_t>;
        using array_type = std::array<T, N>;
        using value_type = T;

        constexpr flut() = delete;

        constexpr T at(std::size_t i) const noexcept
        {
            return data[i];
        }

        constexpr T& at(std::size_t i) noexcept
        {
            return data[i];
        }

        constexpr auto at(const fixed_type& x) const noexcept
        {
            using size_type = array_type::size_type;
            const auto xa = x.template as<size_type>();
            const auto ya = data[xa];
            const auto yb = data[xa + 1];
            return fixed_type::from_raw(ya) + fixed_type::from_raw(yb - ya) * (x - fixed_type{ xa });
        }

        template <typename Callable>
            requires requires(Callable&& f) { f(typename array_type::size_type{ 0 }); }
        static constexpr flut<fixed_type, N> generate(Callable&& f)
        {
            return [](Callable&& f)
            {
                using size_type = array_type::size_type;
                array_type output;
                for (size_type i = 0; i < N; ++i)
                {
                    output[i] = fixed_type{ f(i) }.data();
                }
                return flut<fixed_type, N>{ output };
            }(f);
        }

        template <typename Input_t>
        static constexpr flut<fixed_type, N> make(const std::array<Input_t, N>& input)
        {
            return [](const auto& input)
            {
                using size_type = array_type::size_type;
                array_type output;
                for (size_type i = 0; i < input.size(); ++i)
                {
                    output[i] = fixed_type{ input[i] }.data();
                }
                return flut<fixed_type, N>{ output };
            }(input);
        }

        static constexpr flut<fixed_type, N> make_from_raw(const std::array<T, N>& input)
        {
            return flut<fixed_type, N>{ input };
        }

    private:
        constexpr explicit flut(std::array<T, N>&& arr) : data{ arr }
        {
        }

        constexpr explicit flut(const std::array<T, N>& arr) : data{ arr }
        {
        }

        array_type data;
    };


} // namespace flute
