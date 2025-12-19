#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

class UInt32;
class String;

class Single final : public Object<Single>
{

#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = float;

private:

	value_type Value;

public:

	constexpr Single() : Value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single(T value) noexcept requires(is_promotion_primitive<T>::value) : Value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single(T const& wrapper) noexcept
		: Value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	constexpr Single(Single const&) = default;
	constexpr Single(Single&&) = default;
	constexpr Single& operator=(Single const&) = default;
	constexpr Single& operator=(Single&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
	{
		auto access = Wrapper<T>::Access();
		Value = static_cast<value_type>(other.Value);
		return *this;
	};

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
	constexpr Single const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	constexpr Single operator-() const noexcept {
		return Single(-Value);
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
	constexpr Single& operator++() noexcept {
		++Value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	constexpr Single operator++(int) noexcept {
		return Single(Value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	constexpr Single& operator--() noexcept {
		--Value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	constexpr Single operator--(int) noexcept {
		return Single(Value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value += other;
		return *this.Value;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value -= other.Value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value *= other.Value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Single& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Single& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value /= other.Value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value %= other.Value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value <<= other.Value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value >>= other.Value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value &= other.Value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value |= other.Value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Single& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Single& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value ^= other.Value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value + rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs.Value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value - rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs.Value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value * rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs.Value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value / rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs.Value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs.Value % rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs.Value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator&(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value & rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator&(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs.Value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator|(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value | rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator|(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs.Value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator^(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value ^ rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator^(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs.Value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs.Value << rhs);
		return ToWrapper(R(lhs.Value << rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs.Value >> rhs);
		return ToWrapper(R(lhs.Value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs.Value >> rhs;
		return ToWrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs.Value);
		return ToWrapper(R(lhs >> rhs.Value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

	static inline constexpr Single Epsilon() noexcept { return static_cast<value_type>(1.4e-45f); }
	static inline constexpr Single MaxValue() noexcept { return static_cast<value_type>(3.40282346638528859e+38f); }
	static inline constexpr Single MinValue() noexcept { return static_cast<value_type>(-3.40282346638528859e+38f); }
	static inline constexpr Single Zero() noexcept { return 0.0f; }
	static inline constexpr Single NegativeZero() noexcept { return -0.0f; }
	static inline constexpr Single One() noexcept { return 1.0f; }
	static inline constexpr Single NegativeOne() noexcept { return -1.0f; }
	static inline constexpr Single NaN() noexcept { return NaN_F32(); }
	static inline constexpr Single NegativeInfinity() noexcept { return NegInf_F32(); }
	static inline constexpr Single PositiveInfinity() noexcept { return PosInf_F32(); }

	static inline Boolean IsInfinity(Single const& s) noexcept { return s == Single::PositiveInfinity() || s == Single::NegativeInfinity(); }
	static inline Boolean IsNaN(Single const& s) noexcept { return s != s; }
	static inline Boolean IsNegativeInfinity(Single const& s) noexcept { return s == Single::NegativeInfinity(); }
	static inline Boolean IsPositiveInfinity(Single const& s) noexcept { return s == Single::PositiveInfinity(); }

	Boolean Equals(const Single& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;

private:

	static constexpr float NaN_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0x7FC00000U };
		return u.f; // Quiet NaN IEEE754 (32-bit)
	}

	static constexpr float PosInf_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0x7F800000U };
		return u.f; // +Infinity IEEE754 (32-bit)
	}

	static constexpr float NegInf_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0xFF800000U };
		return u.f; // -Infinity IEEE754 (32-bit)
	}
};

typedef Single f32;