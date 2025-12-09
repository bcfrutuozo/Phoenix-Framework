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

// ---------------------------------------------------------------------
//  is_same
// ---------------------------------------------------------------------
template<typename T, typename U>
struct is_same : false_type {};

template<typename T>
struct is_same<T, T> : true_type {};

template<typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;


typedef char yes_type;
struct no_type { char dummy[2]; };

// Teste: se Derived* pode ser convertido implicitamente para Base*
template <typename Base, typename Derived>
yes_type test_convertible(Base*);

template <typename Base, typename Derived>
no_type test_convertible(...);

template <typename Base, typename Derived>
struct is_base_of
{
    // Remove cv-qualifiers manualmente (sem std::remove_cv)
    using B = const Base;
    using D = const Derived;

    static constexpr bool value =
        sizeof(test_convertible<B, D>(static_cast<D*>(nullptr))) == sizeof(yes_type);
};

template <typename Base, typename Derived>
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;