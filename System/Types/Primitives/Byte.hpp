#pragma once

#include "System/Meta/ValueType.hpp"
#include "System/Types/Fundamentals/Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

class String;

class Byte final : public Object<Byte>
{
#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = unsigned char;

private:

	value_type _value;

public:

	inline constexpr Byte() : _value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte(T value) noexcept requires(is_promotion_primitive<T>::value) : _value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte(T const& wrapper) noexcept
		: _value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	inline constexpr Byte(Byte const&) = default;
	inline constexpr Byte(Byte&&) = default;
	inline constexpr Byte& operator=(Byte const&) = default;
	inline constexpr Byte& operator=(Byte&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { _value = static_cast<value_type>(value); return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) 
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
	inline constexpr operator const size_t() const noexcept { return static_cast<value_type>(_value); }
	inline constexpr operator size_t() noexcept { return static_cast<value_type>(_value); }

	/*
	 * Operator+ (Unary Plus -> Does not change value)
	 */
	inline constexpr Byte const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	inline constexpr Byte operator-() const noexcept {
		return Byte(-_value);
	}

	/*
	 * Operator~ (One's Complement)
	 */
	inline constexpr Byte operator~() const noexcept {
		return Byte(~_value);
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
	inline constexpr Byte& operator++() noexcept {
		++_value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	inline constexpr Byte operator++(int) noexcept {
		return Byte(_value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	inline constexpr Byte& operator--() noexcept {
		--_value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	inline constexpr Byte operator--(int) noexcept {
		return Byte(_value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value += other._value;
		return *this;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value -= other._value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value *= other._value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	inline constexpr Byte& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		_value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	inline constexpr Byte& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		_value /= other._value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value %= other._value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value <<= other._value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value >>= other._value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value &= other._value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value |= other._value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	inline constexpr Byte& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		_value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	inline constexpr Byte& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		_value ^= other._value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value + rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend inline constexpr auto operator+(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs._value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value - rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend inline constexpr auto operator-(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs._value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value * rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend inline constexpr auto operator*(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs._value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs._value / rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend inline constexpr auto operator/(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs._value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs._value % other; return ToWrapper(res); }

	template<typename L, typename R>
	friend inline constexpr auto operator%(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs._value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator&(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value & other;
	}

	template<typename L, typename R>
	friend inline constexpr auto operator&(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator&(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs._value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator|(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value | other;
	}

	template<typename L, typename R>
	friend inline constexpr auto operator|(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator|(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs._value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator^(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs._value ^ other;
	}

	template<typename L, typename R>
	friend inline constexpr auto operator^(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator^(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs._value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs._value << other);
		return ToWrapper(R(lhs._value << other));
	}

	template<typename L, typename R>
	friend inline constexpr auto operator<<(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs._value >> rhs);
		return ToWrapper(R(lhs._value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs._value >> other;
		return ToWrapper(res);
	}

	template<typename L, typename R>
	friend inline constexpr auto operator>>(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs._value);
		return ToWrapper(R(lhs >> rhs._value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value == rhs; }

	template<typename L, typename R>
	friend inline constexpr Boolean operator==(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs._value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs._value == rhs); }

	template<typename L, typename R>
	friend inline constexpr Boolean operator!=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs._value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value < rhs; }

	template<typename L, typename R>
	friend inline constexpr Boolean operator<(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs._value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value <= rhs; }

	template<typename L, typename R>
	friend inline constexpr Boolean operator<=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs._value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value > rhs; }

	template<typename L, typename R>
	friend inline constexpr Boolean operator>(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs._value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs._value >= rhs; }

	template<typename L, typename R>
	friend inline constexpr Boolean operator>=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs._value; }

	static inline constexpr Byte MaxValue() noexcept { return static_cast<value_type>(0xFF); }
	static inline constexpr Byte MinValue() noexcept { return static_cast<value_type>(0x00); }
	static inline constexpr Byte Zero() noexcept { return Byte::MinValue(); }
	static inline constexpr Byte One() noexcept { return static_cast<value_type>(0x01); }

	Boolean Equals(const Byte& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};