#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"

class UInt32;
class String;

class Int32 final : public Object<Int32>
{
public:

	using value_type = int32_t;
	value_type Value;

	constexpr Int32() : Value() {};

	// Construct from promoted primitive types (integral, float, etc.)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32(T value) noexcept requires(is_promotion_primitive<T>::value) : Value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32(T const& wrapper) noexcept
		: Value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	constexpr Int32(Int32 const&) = default;
	constexpr Int32(Int32&&) = default;
	constexpr Int32& operator=(Int32 const&) = default;
	constexpr Int32& operator=(Int32&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32& operator=(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) { Value = static_cast<value_type>(wrapper.Value); return *this; };

	/*
	 * Implicit operator to T() primitives
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr operator T() noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(Value); };

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr operator T() const noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(Value); };

	/*
	 * Operator+ (Unary Plus -> Does not change value)
	 */
	constexpr Int32 const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	constexpr Int32 operator-() const noexcept {
		return Int32(-Value);
	}

	/*
	 * Operator~ (One's Complement)
	 */
	constexpr Int32 operator~() const noexcept {
		return Int32(~Value);
	}

	/*
	 * Operator! (Logical NOT)
	 * Not applicable for int. However, C++ allows its usage
	 */
	constexpr Boolean operator!() const noexcept {
		return !Value;
	}

	/*
	 * Operator++ (Prefix increment)
	 */
	constexpr Int32& operator++() noexcept {
		++Value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	constexpr Int32 operator++(int) noexcept {
		return Int32(Value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	constexpr Int32& operator--() noexcept {
		--Value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	constexpr Int32 operator--(int) noexcept {
		return Int32(Value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value += other.Value;
		return *this;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value -= other.Value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value *= other.Value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Int32& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Int32& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value /= other.Value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value %= other.Value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value <<= other.Value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator>>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator>>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value >>= other.Value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator&=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator&=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value &= other.Value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator|=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator|=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value |= other.Value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Int32& operator^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Int32& operator^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value ^= other.Value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value + rhs; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr auto operator+(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value + rhs.Value; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs.Value; return Wrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value - rhs; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr auto operator-(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value - rhs.Value; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs.Value; return Wrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value * rhs; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr auto operator*(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value * rhs.Value; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs.Value; return Wrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value / rhs; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr auto operator/(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value / rhs.Value; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs.Value; return Wrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs.Value % other; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) { auto res = lhs.Value % other.Value; return Wrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs.Value; return Wrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator&(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value & other;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator&(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		return lhs.Value & other.Value;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator&(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs.Value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator|(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value | other;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator|(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		return lhs.Value | other.Value;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator|(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs.Value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator^(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value ^ other;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator^(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		return lhs.Value ^ other.Value;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator^(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs.Value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs.Value << other);
		return Wrapper(R(lhs.Value << other));
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		using R = decltype(lhs.Value << other.Value);
		return Wrapper(R(lhs.Value << other.Value));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs.Value >> rhs);
		return Wrapper(R(lhs.Value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Int32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs.Value >> other;
		return Wrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Int32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		using R = decltype(lhs.Value >> other.Value);
		return Wrapper(R(lhs.Value >> other.Value));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs.Value);
		return Wrapper(R(lhs >> rhs.Value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value == rhs.Value; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return !(lhs.Value == rhs.Value); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value < rhs.Value; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value <= rhs.Value; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value > rhs.Value; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value >= rhs.Value; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

	static inline constexpr Int32 MaxValue() noexcept { return 0x7FFFFFFF; }
	static inline constexpr Int32 MinValue() noexcept { return 0x80000000; }
	static inline constexpr Int32 Zero() noexcept { return 0x00000000; }
	static inline constexpr Int32 One() noexcept { return 0x00000001; }
	static inline constexpr Int32 NegativeOne() noexcept { return 0xFFFFFFFF; }

	Boolean Equals(const Int32& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};