#pragma once

#include "System/Meta/ValueType.hpp"
#include "System/Types/Fundamentals/Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

class String;

class Int32 final : public Object<Int32>
{

#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = int32_t;

private:

	value_type _value;

public:

	inline constexpr Int32() : _value() {};

	// Construct from promoted primitive types (integral, float, etc.)
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32(T value) noexcept requires(is_promotion_primitive<T>::value) : _value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32(T const& wrapper) noexcept
		: _value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	inline constexpr Int32(Int32 const&) = default;
	inline constexpr Int32(Int32&&) = default;
	inline constexpr Int32& operator=(Int32 const&) = default;
	inline constexpr Int32& operator=(Int32&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { _value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
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
	 * For native array access
	 */
	inline constexpr operator size_t() noexcept { return static_cast<value_type>(_value); }

	/*
	 * Operator+ (Unary Plus -> Does not change value)
	 */
	inline constexpr Int32 const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	inline constexpr Int32 operator-() const noexcept {
		return Int32(-_value);
	}

	/*
	 * Operator~ (One's Complement)
	 */
	inline constexpr Int32 operator~() const noexcept {
		return Int32(~_value);
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
	inline constexpr Int32& operator++() noexcept {
		++_value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	inline constexpr Int32 operator++(int) noexcept {
		return Int32(_value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	inline constexpr Int32& operator--() noexcept {
		--_value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	inline constexpr Int32 operator--(int) noexcept {
		return Int32(_value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value += other._value;
		return *this;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value -= other._value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value *= other._value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Int32& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Int32& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value /= other._value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value %= other._value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value <<= other._value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator>>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator>>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value >>= other._value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator&=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator&=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value &= other._value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator|=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator|=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value |= other._value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Int32& operator^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Int32& operator^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value ^= other._value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value + rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs._value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value - rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs._value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value * rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs._value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value / rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs._value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs._value % rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs._value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator&(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value & rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator&(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs._value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator|(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value | rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator|(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs._value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator^(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value ^ rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Int32 operator^(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs._value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs._value << rhs);
		return ToWrapper(R(lhs._value << rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs._value >> rhs);
		return ToWrapper(R(lhs._value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs._value >> rhs;
		return ToWrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs._value);
		return ToWrapper(R(lhs >> rhs._value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value == rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs._value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs._value == rhs); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs._value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value < rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs._value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value <= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs._value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value > rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs._value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Int32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value >= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Int32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs._value; }

	static inline constexpr Int32 MaxValue() noexcept { return 0x7FFFFFFF; }
	static inline constexpr Int32 MinValue() noexcept { return 0x80000000; }
	static inline constexpr Int32 Zero() noexcept { return 0x00000000; }
	static inline constexpr Int32 One() noexcept { return 0x00000001; }
	static inline constexpr Int32 NegativeOne() noexcept { return 0xFFFFFFFF; }

	Boolean Equals(const Int32& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};

