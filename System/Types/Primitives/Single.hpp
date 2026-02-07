#pragma once

#include "System/Meta/ValueType.hpp"
#include "System/Types/Fundamentals/Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

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

	value_type _value;

public:

	inline constexpr Single() : _value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single(T value) noexcept requires(is_promotion_primitive<T>::value) : _value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single(T const& wrapper) noexcept
		: _value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	inline constexpr Single(Single const&) = default;
	inline constexpr Single(Single&&) = default;
	inline constexpr Single& operator=(Single const&) = default;
	inline constexpr Single& operator=(Single&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { _value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
	{
		auto access = Wrapper<T>::Access();
		_value = static_cast<value_type>(other._value);
		return *this;
	};

	/*
	 * Implicit operator to T() primitives
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr operator T() noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(_value); };

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr operator T() const noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(_value); };

	/*
	 * Operator+ (Unary Plus -> Does not change value)
	 */
	inline constexpr Single const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	inline constexpr Single operator-() const noexcept {
		return Single(-_value);
	}

	/*
	 * Operator! (Logical NOT)
	 * Not applicable for int. However, C++ allows its usage
	 */
	inline constexpr Boolean operator!() const noexcept {
		return !_value;
	}

	/*
	 * Operator++ (Prefix increment)
	 */
	inline constexpr Single& operator++() noexcept {
		++_value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	inline constexpr Single operator++(int) noexcept {
		return Single(_value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	inline constexpr Single& operator--() noexcept {
		--_value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	inline constexpr Single operator--(int) noexcept {
		return Single(_value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value += other;
		return *this._value;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value -= other._value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value *= other._value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Single& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Single& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value /= other._value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value %= other._value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value <<= other._value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value >>= other._value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value &= other._value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value |= other._value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Single& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Single& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value ^= other._value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value + rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs._value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value - rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs._value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value * rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs._value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value / rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs._value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs._value % rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs._value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator&(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value & rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator&(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs._value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator|(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value | rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator|(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs._value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator^(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value ^ rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Single operator^(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs._value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs._value << rhs);
		return ToWrapper(R(lhs._value << rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs._value >> rhs);
		return ToWrapper(R(lhs._value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs._value >> rhs;
		return ToWrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs._value);
		return ToWrapper(R(lhs >> rhs._value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value == rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs._value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs._value == rhs); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs._value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value < rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs._value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value <= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs._value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value > rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs._value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Single const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value >= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Single const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs._value; }

	static inline constexpr Single Epsilon() noexcept { return static_cast<value_type>(1.1920928955078125e-7f); }
	static inline constexpr Single MaxValue() noexcept { return static_cast<value_type>(3.40282346638528859e+38f); }
	static inline constexpr Single MinValue() noexcept { return static_cast<value_type>(1.1754943508222875e-38f); }
	static inline constexpr Single Lowest() noexcept { return static_cast<value_type>(-3.4028234663852886e+38f); }
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

	static inline constexpr float NaN_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0x7FC00000U };
		return u.f; // Quiet NaN IEEE754 (32-bit)
	}

	static inline constexpr float PosInf_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0x7F800000U };
		return u.f; // +Infinity IEEE754 (32-bit)
	}

	static inline constexpr float NegInf_F32() noexcept
	{
		union { unsigned int u; float f; } u = { 0xFF800000U };
		return u.f; // -Infinity IEEE754 (32-bit)
	}
};

typedef Single f32;