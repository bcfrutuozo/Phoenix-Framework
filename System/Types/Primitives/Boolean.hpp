#pragma once

#include "System/Meta/ValueType.hpp"
#include "System/Types/Fundamentals/Object.hpp"
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

	value_type _value;

public:
	
	inline constexpr Boolean() noexcept : _value(false) {}
	inline constexpr explicit Boolean(bool v) noexcept : _value(v) {}

	// Construct from promoted primitive types (integral, float, etc.)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Boolean(T v) noexcept requires(is_promotion_primitive<T>::value)
		: _value(static_cast<value_type>(v)) {
	}

	// Construct from promoted wrapper types
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Boolean(T const& wrapper) noexcept
		: _value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	inline constexpr Boolean(Boolean const&) noexcept = default;
	inline constexpr Boolean(Boolean&&) noexcept = default;
	inline constexpr Boolean& operator=(Boolean const&) noexcept = default;
	inline constexpr Boolean& operator=(Boolean&&) noexcept = default;

	// Assignment from promoted primitive
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Boolean& operator=(T value) noexcept requires(is_promotion_primitive<T>::value) 
	{
		_value = static_cast<value_type>(value);
		return *this;
	}

	// Assignment from promoted wrapper
	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Boolean& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
	{
		auto access = Wrapper<T>::Access();
		_value = static_cast<value_type>(other._value);
		return *this;
	}

	// conversion to native bool (implicit, to use in if(), assert(), etc.)
	inline constexpr operator bool() const noexcept { return _value; }

	// Only provide a single template conversion operator (const) to primitives/wrappers
	// This avoids MSVC ambiguities with duplicate template operators.
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr operator T() const noexcept requires(is_promotion_primitive<T>::value) {
		// Converting bool -> primitive: false -> 0, true -> 1 (as usual in C++)
		return static_cast<T>(_value ? 1 : 0);
	}

	// logical NOT returns native bool so `!booleanWrapper` works in `if`, `assert`, etc.
	inline constexpr bool operator!() const noexcept { return !_value; }

	// --- logical operators (friend) --------------------------------------

	// Logical AND / OR implemented in terms of native bool semantics.
	// These do not short-circuit because operator overloads cannot mimic short-circuiting,
	// but they provide consistent semantics with the wrapper type.
	friend inline constexpr Boolean operator&&(Boolean const& lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs._value && rhs._value);
	}
	friend inline constexpr Boolean operator||(Boolean const& lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs._value || rhs._value);
	}

	// Overloads to mix with native bool / primitives (promoted primitives)
	friend inline constexpr Boolean operator&&(Boolean const& lhs, bool rhs) noexcept {
		return Boolean(lhs._value && rhs);
	}
	friend inline constexpr Boolean operator&&(bool lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs && rhs._value);
	}

	friend inline constexpr Boolean operator||(Boolean const& lhs, bool rhs) noexcept {
		return Boolean(lhs._value || rhs);
	}
	friend inline constexpr Boolean operator||(bool lhs, Boolean const& rhs) noexcept {
		return Boolean(lhs || rhs._value);
	}

	// --- comparisons -----------------------------------------------------

	// Compare with Boolean -> return native bool for idiomatic comparisons
	friend inline constexpr Boolean operator==(Boolean const& a, Boolean const& b) noexcept {
		return a._value == b._value;
	}
	friend inline constexpr Boolean operator!=(Boolean const& a, Boolean const& b) noexcept {
		return a._value != b._value;
	}

	// Compare with promoted primitive types (returns Boolean to follow your API style)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Boolean const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(lhs._value == static_cast<value_type>(rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Boolean const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(static_cast<value_type>(lhs) == rhs._value);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Boolean const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(lhs._value != static_cast<value_type>(rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Boolean const& rhs) noexcept requires(is_promotion_primitive<T>::value) {
		return Boolean(static_cast<value_type>(lhs) != rhs._value);
	}

	// Compare with promoted wrapper types (assumes those wrappers can convert to bool)
	template<typename L, typename R >
	friend inline constexpr Boolean operator==(L const& lhs, R const& rhs) noexcept;

	template<typename L, typename R>
	friend inline constexpr Boolean operator!=(L const& lhs, R const& rhs) noexcept;

	Boolean Equals(const Boolean& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};