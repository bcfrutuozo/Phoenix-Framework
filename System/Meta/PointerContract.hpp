#pragma once

//
// PointerContract.hpp
//
// Compile-time structural contract for Pointer.
//
// This file enforces that Pointer:
//  - is a trivial, standard-layout type
//  - has no virtuals
//  - has pointer-sized storage and alignment
//  - has no hidden ownership or behavior
//  - is safe to pass across ABI boundaries
//
// IMPORTANT:
//  - Pointer MUST NOT include this file.
//  - This file defines NO runtime behavior.
//  - This file exists only to validate invariants.
//

#include "../Types/Pointer.hpp"
#include "TypeTraits.hpp"
#include "BaseDef.hpp"

// ------------------------------------------------------------
// Size & alignment
// ------------------------------------------------------------

static_assert(sizeof(Pointer) == sizeof(void*),
    "Pointer must be exactly pointer-sized");

static_assert(alignof(Pointer) == alignof(void*),
    "Pointer alignment must match native pointer alignment");

// ------------------------------------------------------------
// Triviality & layout
// ------------------------------------------------------------

static_assert(is_trivially_copyable<Pointer>::value,
    "Pointer must be trivially copyable");

static_assert(is_trivially_destructible<Pointer>::value,
    "Pointer must be trivially destructible");

static_assert(is_standard_layout<Pointer>::value,
    "Pointer must have standard layout");

// ------------------------------------------------------------
// Object semantics
// ------------------------------------------------------------

// No polymorphism
static_assert(!is_polymorphic<Pointer>::value,
    "Pointer must not be polymorphic");

// ------------------------------------------------------------
// Construction invariants
// ------------------------------------------------------------

// Default construction must yield null
static_assert(Pointer().IsNull(),
    "Default-constructed Pointer must be null");

// Nullptr construction must yield null
static_assert(Pointer(decltype(nullptr){}).IsNull(),
    "nullptr-constructed Pointer must be null");

// ------------------------------------------------------------
// Conversion invariants
// ------------------------------------------------------------

// Pointer must be constructible from raw value
static_assert(is_constructible<
    Pointer,
    Pointer::value_type
>::value,
    "Pointer must be constructible from its value_type");

// Pointer must be constructible from void*
static_assert(is_constructible<
    Pointer,
    const void*
>::value,
    "Pointer must be constructible from const void*");

// Pointer must NOT be implicitly convertible from void*
static_assert(!is_convertible<
    const void*,
    Pointer
>::value,
    "Pointer construction from void* must be explicit");

// Pointer must NOT be implicitly convertible to void*
static_assert(!is_convertible<
    Pointer,
    void*
>::value,
    "Pointer must not implicitly convert to void*");

// Pointer must be explicitly testable in boolean context
static_assert(
    is_boolean_testable<Pointer>::value,
    "Pointer must be testable in boolean context"
    );

// ------------------------------------------------------------
// Equality semantics
// ------------------------------------------------------------

static_assert(is_equality_comparable<Pointer>::value,
    "Pointer must be equality comparable");

// ------------------------------------------------------------
// ABI safety
// ------------------------------------------------------------

// Must be safe to memcpy / pass through C ABI
static_assert(is_trivially_copyable<Pointer>::value,
    "Pointer must be ABI-safe");

// ------------------------------------------------------------
// End of contract
// ------------------------------------------------------------