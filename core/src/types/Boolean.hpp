#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Operators.hpp"

class String;

class Boolean final : public Object<Boolean>
{
#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = bool;

private:

	value_type Value;

public:
	
	constexpr Boolean() noexcept : Value(false) {}
	constexpr explicit Boolean(bool v) noexcept : Value(v) {}

	// Construct from promoted primitive types (integral, float, etc.)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Boolean(T v) noexcept requires(is_promotion_primitive<T>::value)
		: Value(static_cast<value_type>(v)) {
	}

	// Construct from promoted wrapper types
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Boolean(T const& wrapper) noexcept
		: Value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	constexpr Boolean(Boolean const&) noexcept = default;
	constexpr Boolean(Boolean&&) noexcept = default;
	constexpr Boolean& operator=(Boolean const&) noexcept = default;
	constexpr Boolean& operator=(Boolean&&) noexcept = default;

	// Assignment from promoted primitive
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Boolean& operator=(T value) noexcept requires(is_promotion_primitive<T>::value) 
	{
		Value = static_cast<value_type>(value);
		return *this;
	}

	// Assignment from promoted wrapper
	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Boolean& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
	{
		auto access = Wrapper<T>::Access();
		Value = static_cast<value_type>(other.Value);
		return *this;
	}

	// conversion to native bool (implicit, to use in if(), assert(), etc.)
	constexpr operator bool() const noexcept { return Value; }

	// Only provide a single template conversion operator (const) to primitives/wrappers
	// This avoids MSVC ambiguities with duplicate template operators.
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr operator T() const noexcept requires(is_promotion_primitive<T>::value) {
		// Converting bool -> primitive: false -> 0, true -> 1 (as usual in C++)
		return static_cast<T>(Value ? 1 : 0);
	}

	// logical NOT returns native bool so `!booleanWrapper` works in `if`, `assert`, etc.
	constexpr bool operator!() const noexcept { return !Value; }

	// --- logical operators (friend) --------------------------------------

	// Logical AND / OR implemented in terms of native bool semantics.
	// These do not short-circuit because operator overloads cannot mimic short-circuiting,
	// but they provide consistent semantics with the wrapper type.
	friend constexpr Boolean operator&&(Boolean const& lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs.Value && rhs.Value);
	}
	friend constexpr Boolean operator||(Boolean const& lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs.Value || rhs.Value);
	}

	// Overloads to mix with native bool / primitives (promoted primitives)
	friend constexpr Boolean operator&&(Boolean const& lhs, bool rhs) noexcept {
		return Boolean(lhs.Value && rhs);
	}
	friend constexpr Boolean operator&&(bool lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs && rhs.Value);
	}

	friend constexpr Boolean operator||(Boolean const& lhs, bool rhs) noexcept {
		return Boolean(lhs.Value || rhs);
	}
	friend constexpr Boolean operator||(bool lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs || rhs.Value);
	}

	// --- comparisons -----------------------------------------------------

	// Compare with Boolean -> return native bool for idiomatic comparisons
	friend constexpr Boolean operator==(Boolean const& a, Boolean const& b) noexcept {
		return a.Value == b.Value;
	}
	friend constexpr Boolean operator!=(Boolean const& a, Boolean const& b) noexcept {
		return a.Value != b.Value;
	}

	// Compare with promoted primitive types (returns Boolean to follow your API style)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Boolean const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(lhs.Value == static_cast<value_type>(rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Boolean const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(static_cast<value_type>(lhs) == rhs.Value);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Boolean const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(lhs.Value != static_cast<value_type>(rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Boolean const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(static_cast<value_type>(lhs) != rhs.Value);
	}

	// Compare with promoted wrapper types (assumes those wrappers can convert to bool)
	template<typename L, typename R >
	friend constexpr Boolean operator==(L const& lhs, R const& rhs) noexcept;

	template<typename L, typename R>
	friend constexpr Boolean operator!=(L const& lhs, R const& rhs) noexcept;

	Boolean Equals(const Boolean& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};