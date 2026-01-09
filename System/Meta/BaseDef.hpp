#pragma once

// ---------------------------------------------------------------------
//  true_type / false_type / bool_constant
// ---------------------------------------------------------------------

struct false_type {
    static constexpr bool value = false;
    using value_type = bool;
    using type = false_type;
    constexpr operator value_type() const noexcept { return value; }
};

struct true_type {
    static constexpr bool value = true;
    using value_type = bool;
    using type = true_type;
    constexpr operator value_type() const noexcept { return value; }
};

template<bool B>
struct bool_constant : false_type {};

template<>
struct bool_constant<true> : true_type {};

template<bool B>
inline constexpr bool bool_constant_v = bool_constant<B>::value;

// ---------------------------------------------------------------------
//  enable_if
// ---------------------------------------------------------------------

template<bool B, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T> { using type = T; };

template<bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template<typename T>
T&& declval() noexcept;