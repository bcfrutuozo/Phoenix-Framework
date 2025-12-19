#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"
#include "Operators.hpp"

class UInt32;
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

	value_type Value;

public:

	constexpr Byte() : Value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte(T value) noexcept requires(is_promotion_primitive<T>::value) : Value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte(T const& wrapper) noexcept
		: Value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	constexpr Byte(Byte const&) = default;
	constexpr Byte(Byte&&) = default;
	constexpr Byte& operator=(Byte const&) = default;
	constexpr Byte& operator=(Byte&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = static_cast<value_type>(value); return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) 
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
	constexpr Byte const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator- (Unary Minus -> Changes the sign of value)
	 */
	constexpr Byte operator-() const noexcept {
		return Byte(-Value);
	}

	/*
	 * Operator~ (One's Complement)
	 */
	constexpr Byte operator~() const noexcept {
		return Byte(~Value);
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
	constexpr Byte& operator++() noexcept {
		++Value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	constexpr Byte operator++(int) noexcept {
		return Byte(Value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	constexpr Byte& operator--() noexcept {
		--Value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	constexpr Byte operator--(int) noexcept {
		return Byte(Value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value += other.Value;
		return *this;
	}

	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value -= other.Value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value *= other.Value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr Byte& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr Byte& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value /= other.Value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value %= other.Value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value <<= other.Value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value >>= other.Value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value &= other.Value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value |= other.Value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr Byte& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr Byte& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value ^= other.Value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value + rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend constexpr auto operator+(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs.Value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value - rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend constexpr auto operator-(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs.Value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value * rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend constexpr auto operator*(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs.Value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value / rhs; return ToWrapper(res); }

	template<typename L, typename R>
	friend constexpr auto operator/(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs.Value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs.Value % other; return ToWrapper(res); }

	template<typename L, typename R>
	friend constexpr auto operator%(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs.Value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator&(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value & other;
	}

	template<typename L, typename R>
	friend constexpr auto operator&(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator&(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs.Value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator|(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value | other;
	}

	template<typename L, typename R>
	friend constexpr auto operator|(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator|(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs.Value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator^(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value ^ other;
	}

	template<typename L, typename R>
	friend constexpr auto operator^(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr Byte operator^(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs.Value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs.Value << other);
		return ToWrapper(R(lhs.Value << other));
	}

	template<typename L, typename R>
	friend constexpr auto operator<<(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs.Value >> rhs);
		return ToWrapper(R(lhs.Value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(Byte const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs.Value >> other;
		return ToWrapper(res);
	}

	template<typename L, typename R>
	friend constexpr auto operator>>(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs.Value);
		return ToWrapper(R(lhs >> rhs.Value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

	template<typename L, typename R>
	friend constexpr Boolean operator==(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

	template<typename L, typename R>
	friend constexpr Boolean operator!=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

	template<typename L, typename R>
	friend constexpr Boolean operator<(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

	template<typename L, typename R>
	friend constexpr Boolean operator<=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

	template<typename L, typename R>
	friend constexpr Boolean operator>(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(Byte const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

	template<typename L, typename R>
	friend constexpr Boolean operator>=(L const& lhs, R const& rhs) noexcept;

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, Byte const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

	static inline constexpr Byte MaxValue() noexcept { return static_cast<value_type>(0xFF); }
	static inline constexpr Byte MinValue() noexcept { return static_cast<value_type>(0x00); }
	static inline constexpr Byte Zero() noexcept { return Byte::MinValue(); }
	static inline constexpr Byte One() noexcept { return static_cast<value_type>(0x01); }

	Boolean Equals(const Byte& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};