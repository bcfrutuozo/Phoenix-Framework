#pragma once

#include "System/Meta/ValueType.hpp"
#include "System/Types/Fundamentals/Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

class String;

class SByte final : public Object<SByte>
{

#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = int8_t;

private:

	value_type _value;

public:

	inline constexpr SByte() : _value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte(T value) noexcept requires(is_promotion_primitive<T>::value) : _value((value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte(T const& wrapper) noexcept
		: _value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	inline constexpr SByte(SByte const&) = default;
	inline constexpr SByte(SByte&&) = default;
	inline constexpr SByte& operator=(SByte const&) = default;
	inline constexpr SByte& operator=(SByte&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { _value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
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
	inline constexpr SByte const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	inline constexpr SByte operator-() const noexcept {
		return SByte(-_value);
	}

	/*
	 * Operator~ (One's Complement)
	 */
	inline constexpr SByte operator~() const noexcept {
		return SByte(~_value);
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
	inline constexpr SByte& operator++() noexcept {
		++_value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	inline constexpr SByte operator++(int) noexcept {
		return SByte(_value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	inline constexpr SByte& operator--() noexcept {
		--_value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	inline constexpr SByte operator--(int) noexcept {
		return SByte(_value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value += other._value;
		return *this;
	}


	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value -= other._value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value *= other._value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr SByte& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr SByte& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value /= other._value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value %= other._value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value <<= other._value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value >>= other._value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value &= other._value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value |= other._value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr SByte& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr SByte& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value ^= other._value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value + rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs._value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value - rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs._value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value * rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs._value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value / rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs._value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs._value % rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs._value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator&(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value & rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator&(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs._value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator|(SByte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value | other;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator|(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs._value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator^(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value ^ rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr SByte operator^(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs._value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs._value << rhs);
		return ToWrapper(R(lhs._value << rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs._value >> rhs);
		return ToWrapper(R(lhs._value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs._value >> rhs;
		return ToWrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs._value);
		return ToWrapper(R(lhs >> rhs._value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value == rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs._value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs._value == rhs); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs._value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value < rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs._value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value <= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs._value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value > rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs._value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(SByte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value >= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, SByte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs._value; }

	static const SByte MaxValue;
	static const SByte MinValue;
	static const SByte Zero;
	static const SByte One;
	static const SByte NegativeOne;

	Boolean Equals(const SByte& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};

inline const SByte SByte::MaxValue = static_cast<SByte::value_type>(0x7F);
inline const SByte SByte::MinValue = static_cast<SByte::value_type>(0x80);
inline const SByte SByte::Zero = static_cast<SByte::value_type>(0x00);
inline const SByte SByte::One = static_cast<SByte::value_type>(0x01);
inline const SByte SByte::NegativeOne = static_cast<SByte::value_type>(0xFF);