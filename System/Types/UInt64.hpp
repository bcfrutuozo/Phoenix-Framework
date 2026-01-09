#pragma once

#include "System/Meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"
#include "Byte.hpp"
#include "Operators.hpp"

class UInt32;
class String;

class UInt64 final : public Object<UInt64>
{

#define WRAPPER(name) friend class name;
#include "Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

	using value_type = uint64_t;

private:

	value_type Value;

public:

	constexpr UInt64() : Value() {};

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64(T value) noexcept requires(is_promotion_primitive<T>::value) : Value(static_cast<value_type>(value)) {}

	/*
	 * Constructor which receives another Wrapper
	 * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
	 * by its operator T() function
	 */
	template<typename T,
		enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64(T const& wrapper) noexcept
		: Value(static_cast<value_type>(
			static_cast<typename T::value_type>(wrapper))) {
	}

	constexpr UInt64(UInt64 const&) = default;
	constexpr UInt64(UInt64&&) = default;
	constexpr UInt64& operator=(UInt64 const&) = default;
	constexpr UInt64& operator=(UInt64&&) = default;

	/*
	 * Operator= (Assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64& operator=(T const& other) noexcept requires(is_promotion_wrapper<T>::value)
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

	// Implicit convertion for size_t since it is the default type
	constexpr operator size_t() const noexcept {
		return static_cast<size_t>(Value);
	}

	/*
	 * Operator[] to extract bytes
	 */
	constexpr Byte operator[](int idx) const noexcept {
		return static_cast<Byte>((Value >> (idx * 8)) & 0xFF);
	}

	/*
	 * Operator+ (Unary Plus -> Does not change value)
	 */
	constexpr UInt64 const& operator+() const noexcept {
		return *this;
	}

	/*
	 * Operator~ (One's Complement)
	 */
	constexpr UInt64 operator~() const noexcept {
		return UInt64(~Value);
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
	constexpr UInt64& operator++() noexcept {
		++Value;
		return *this;
	}

	/*
	 * Operator++ (Postfix increment)
	 */
	constexpr UInt64 operator++(int) noexcept {
		return UInt64(Value++);
	}

	/*
	 * Operator-- (Prefix decrement)
	 */
	constexpr UInt64& operator--() noexcept {
		--Value;
		return *this;
	}

	/*
	 * Operator-- (Postfix decrement)
	 */
	constexpr UInt64 operator--(int) noexcept {
		return UInt64(Value--);
	}

	/*
	 * Operator+= (Addition assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value += other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value += other.Value;
		return *this;
	}


	/*
	 * Operator-= (Subtraction assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value -= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value -= other.Value;
		return *this;
	}

	/*
	 * Operator*= (Multiplication assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value *= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value *= other.Value;
		return *this;
	}

	/*
	 * Operator/= (Division assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	constexpr UInt64& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
		Value /= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
	constexpr UInt64& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
		Value /= other.Value;
		return *this;
	}

	/*
	 * Operator%= (Modulo assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value %= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value %= other.Value;
		return *this;
	}

	/*
	 * Operator<<= (Shift-Left assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value <<= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value <<= other.Value;
		return *this;
	}

	/*
	 * Operator>>= (Shift-Right assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value >>= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value >>= other.Value;
		return *this;
	}

	/*
	 * Operator&= (Logical AND assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value &= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value &= other.Value;
		return *this;
	}

	/*
	 * Operator|= (Logical OR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value |= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value |= other.Value;
		return *this;
	}

	/*
	 * Operator^= (Logical XOR assignment)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	constexpr UInt64& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		Value ^= other;
		return *this;
	}

	template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
	constexpr UInt64& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
		Value ^= other.Value;
		return *this;
	}

	/*
	 * Operator+ (Addition)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value + rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator+(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs.Value; return ToWrapper(res); }

	/*
	 * Operator- (Subtraction)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value - rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator-(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs.Value; return ToWrapper(res); }

	/*
	* Operator* (Multiplication)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value * rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator*(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs.Value; return ToWrapper(res); }

	/*
	* Operator/ (Division)
	*/
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value / rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr auto operator/(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs.Value; return ToWrapper(res); }

	/*
	 * Operator% (Modulo)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs.Value % rhs; return ToWrapper(res); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator%(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs.Value; return ToWrapper(res); }

	/*
	 * Operator& (Logical AND)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator&(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value & rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator&(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs & rhs.Value;
	}

	/*
	 * Operator| (Logical OR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator|(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value | rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator|(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs | rhs.Value;
	}

	/*
	 * Operator^ (Logical XOR)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator^(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		return lhs.Value ^ rhs;
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr UInt64 operator^(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		return lhs ^ rhs.Value;
	}

	/*
	 * Operator<< (Shift-Left)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		using R = decltype(lhs.Value << rhs);
		return ToWrapper(R(lhs.Value << rhs));
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator<<(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs.Value >> rhs);
		return ToWrapper(R(lhs.Value >> rhs));
	}

	/*
	 * Operator>> (Shift-Right)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
		auto res = lhs.Value >> rhs;
		return ToWrapper(res);
	}

	template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
	friend inline constexpr auto operator>>(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
	{
		using R = decltype(lhs >> rhs.Value);
		return ToWrapper(R(lhs >> rhs.Value));
	}

	/*
	 * Operator== (Equality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator==(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

	/*
	 * Operator!= (Inequality)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator!=(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

	/*
	 * Operator< (Less Than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

	/*
	 * Operator<= (Less Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator<=(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

	/*
	 * Operator> (Greater than)
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

	/*
	 * Operator>= (Greater Than or Equal to )
	 */
	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(UInt64 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

	template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
	friend inline constexpr Boolean operator>=(T const& lhs, UInt64 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

	static inline constexpr UInt64 MaxValue() noexcept { return static_cast<value_type>(0XFFFFFFFFFFFFFFFFLL); }
	static inline constexpr UInt64 MinValue() noexcept { return static_cast<value_type>(0x8000000000000000LL); }
	static inline constexpr UInt64 Zero() noexcept { return static_cast<value_type>(0x0000000000000000LL); }
	static inline constexpr UInt64 One() noexcept { return static_cast<value_type>(0x0000000000000001LL); }

	Boolean Equals(const UInt64& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;
};

