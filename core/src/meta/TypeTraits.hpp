#pragma once

#include <cstdint>
#include "BaseDef.hpp"

// ---------------------------------------------------------------------
//  is_same
// ---------------------------------------------------------------------
template<typename T, typename U>
struct is_same : false_type {};

template<typename T>
struct is_same<T, T> : true_type {};

template<typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

// ---------------------------------------------------------------------
//  is_integral
// ---------------------------------------------------------------------

template<typename T>
struct is_integral : false_type {};

template<> struct is_integral<bool> : true_type {};
template<> struct is_integral<char> : true_type {};
template<> struct is_integral<signed char> : true_type {};
template<> struct is_integral<unsigned char> : true_type {};
template<> struct is_integral<short> : true_type {};
template<> struct is_integral<unsigned short> : true_type {};
template<> struct is_integral<int> : true_type {};
template<> struct is_integral<unsigned int> : true_type {};
template<> struct is_integral<long> : true_type {};
template<> struct is_integral<unsigned long> : true_type {};
template<> struct is_integral<long long> : true_type {};
template<> struct is_integral<unsigned long long> : true_type {};

template<typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

// ---------------------------------------------------------------------
//  is_signed
// ---------------------------------------------------------------------
template<typename T>
struct is_signed : false_type {};

template<> struct is_signed<signed char> : true_type {};
template<> struct is_signed<short> : true_type {};
template<> struct is_signed<int> : true_type {};
template<> struct is_signed<long> : true_type {};
template<> struct is_signed<long long> : true_type {};
template<> struct is_signed<char> : true_type {}; // dependendo do compilador, char pode ser signed

template<typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;

// ---------------------------------------------------------------------
//  is_unsigned
// ---------------------------------------------------------------------
template<typename T>
struct is_unsigned : false_type {};

template<> struct is_unsigned<unsigned char> : true_type {};
template<> struct is_unsigned<unsigned short> : true_type {};
template<> struct is_unsigned<unsigned int> : true_type {};
template<> struct is_unsigned<unsigned long> : true_type {};
template<> struct is_unsigned<unsigned long long> : true_type {};

template<typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

// ---------------------------------------------------------------------
//  is_floating_point
// ---------------------------------------------------------------------
template<typename T>
struct is_floating_point : false_type {};

template<> struct is_floating_point<float> : true_type {};
template<> struct is_floating_point<double> : true_type {};
template<> struct is_floating_point<long double> : true_type {};

template<typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template<typename T>
struct is_trivially_copyable
    : bool_constant<__is_trivially_copyable(T)> {
};

template<typename T>
struct is_trivially_destructible
    : bool_constant<__is_trivially_destructible(T)> {
};

template<typename T>
struct is_standard_layout
    : bool_constant<__is_standard_layout(T)> {
};

template<typename T>
struct is_trivial
    : bool_constant<__is_trivial(T)> {
};

template<typename T>
struct is_polymorphic
    : bool_constant<__is_polymorphic(T)> {
};

template<typename T, typename... Args>
struct is_constructible
{
private:
    template<typename U, typename... A>
    static auto test(int) -> decltype(U(declval<A>()...), true_type{});

    template<typename, typename...>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T, Args...>(0))::value;
};

template<typename From, typename To>
struct is_convertible
{
private:
    static void accept(To);

    template<typename F>
    static auto test(int) -> decltype(accept(declval<F>()), true_type{});

    template<typename>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<From>(0))::value;
};

template<typename T>
struct is_equality_comparable
{
private:
    template<typename U>
    static auto test(int) -> decltype(
        declval<U>() == declval<U>(),
        true_type{}
        );

    template<typename>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
struct is_boolean_testable
{
private:
    template<typename U>
    static auto test(int) -> decltype(
        static_cast<bool>(declval<U>()),
        true_type{}
        );

    template<typename>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// ------------------------------------------------------------
// Copy constructible
// ------------------------------------------------------------

template<typename T>
struct is_copy_constructible
    : bool_constant<__is_constructible(T, const T&)> {
};

// ------------------------------------------------------------
// Move constructible
// ------------------------------------------------------------

template<typename T>
struct is_move_constructible
    : bool_constant<__is_constructible(T, T&&)> {
};