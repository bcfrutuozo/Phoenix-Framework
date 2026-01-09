#pragma once

//
// WrapperContract.hpp
//
// Compile-time structural contract for zero-overhead wrappers.
//
// This file enforces that a wrapper:
//  - defines a public `value_type`
//  - exposes its underlying value via WrapperAccess
//  - is implicitly convertible to value_type
//  - has zero size and alignment overhead
//
// IMPORTANT:
//  - Wrappers MUST NOT include this file.
//  - This file defines NO runtime behavior.
//  - This file exists only to validate invariants.
//

#include "BaseDef.hpp"
#include "Access.hpp"

// ------------------------------------------------------------
// declval (local, no std)
// ------------------------------------------------------------
template<typename T>
T&& declval() noexcept;

// ------------------------------------------------------------
// Detection helpers
// ------------------------------------------------------------

// has value_type
template<typename T, typename = void>
struct has_value_type : false_type {};

template<typename T>
struct has_value_type<T, decltype((void)sizeof(typename T::value_type))>
    : true_type {
};

// has WrapperAccess::get(T&)
template<typename T, typename = void>
struct has_wrapper_access : false_type {};

template<typename T>
struct has_wrapper_access<T, decltype(
    (void)WrapperAccess<T>::get(
        declval<T&>(),
        Wrapper<T>::Access()
    )
    )> : true_type {
};

// convertible to value_type
template<typename T, typename = void>
struct is_value_convertible : false_type {};

template<typename T>
struct is_value_convertible<
    T,
    decltype((void)static_cast<typename T::value_type>(declval<T>()))
> : true_type {
};

// ------------------------------------------------------------
// Wrapper contract
// ------------------------------------------------------------
template<typename T>
struct WrapperContract
{
    // Structural requirements
    static_assert(has_value_type<T>::value,
        "Wrapper must define a public `using value_type = ...`");

    static_assert(has_wrapper_access<T>::value,
        "Wrapper must expose its underlying value via WrapperAccess<T>::get(T&)");

    static_assert(is_value_convertible<T>::value,
        "Wrapper must be implicitly convertible to value_type");

    // Zero-overhead guarantees
    static_assert(sizeof(T) == sizeof(typename T::value_type),
        "Wrapper must have zero size overhead");

    static_assert(alignof(T) == alignof(typename T::value_type),
        "Wrapper alignment must match value_type");

    static_assert(sizeof(T) > 0,
        "Wrapper must not be an empty type");
};

// ------------------------------------------------------------
// Helper macro
// ------------------------------------------------------------
#define VALIDATE_WRAPPER(T) \
    static_assert(true, "Validating wrapper: " #T); \
    template struct WrapperContract<T>