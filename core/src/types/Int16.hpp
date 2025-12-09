#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"

class UInt32;
class String;

class Int16 final : public Object<Int16> {

private:
    
    using value_type = int16_t;
    value_type Value;

public:

    constexpr Int16() : Value() {};

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16(T value) noexcept requires(is_promotion_primitive<T>::value) : Value((value)) {}

    /*
     * Constructor which receives another Wrapper
     * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
     * by its operator T() function
     */
    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr explicit Int16(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) : Value(wrapper) {}

    constexpr Int16(Int16 const&) = default;

    constexpr Int16(Int16&&) = default;

    constexpr Int16& operator=(Int16 const&) = default;

    constexpr Int16& operator=(Int16&&) = default;

    /*
     * Operator= (Assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr Int16& operator=(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) { Value = wrapper; return *this; };

    /*
     * Implicit use of between Primitive and Wrapper
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr operator T() noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(Value); };

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr operator T() noexcept requires(is_promotion_wrapper<T>::value) { return static_cast<T>(Value); };

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr operator T() const noexcept requires(is_promotion_primitive<T>::value) { return static_cast<T>(Value); };

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr operator T() const noexcept requires(is_promotion_wrapper<T>::value) { return static_cast<T>(Value); };

    /*
     * Operator+ (Unary Plus -> Does not change value)
     */
    constexpr Int16 const& operator+() const noexcept {
        return *this;
    }

    /*
     * Operator- (Unary Minus -> Changes the sign of value)
     */
    constexpr Int16 operator-() const noexcept {
        return Int16(-Value);
    }

    /*
     * Operator~ (One's Complement)
     */
    constexpr Int16 operator~() const noexcept {
        return Int16(~Value);
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
    constexpr Int16& operator++() noexcept {
        ++Value;
        return *this;
    }

    /*
     * Operator++ (Postfix increment)
     */
    constexpr Int16 operator++(int) noexcept {
        return Int16(Value++);
    }

    /*
     * Operator-- (Prefix decrement)
     */
    constexpr Int16& operator--() noexcept {
        --Value;
        return *this;
    }

    /*
     * Operator-- (Postfix decrement)
     */
    constexpr Int16 operator--(int) noexcept {
        return Int16(Value--);
    }

    /*
     * Operator+= (Addition assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value += other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr Int16& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value += other;
        return *this;
    }


    /*
     * Operator-= (Subtraction assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value -= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr Int16& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value -= other;
        return *this;
    }

    /*
     * Operator*= (Multiplication assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value *= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr Int16& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value *= other;
        return *this;
    }

    /*
     * Operator/= (Division assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr Int16& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value /= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr Int16& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value /= other;
        return *this;
    }

    /*
     * Operator%= (Modulo assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value %= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value %= other;
        return *this;
    }

    /*
     * Operator<<= (Shift-Left assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value <<= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value <<= other;
        return *this;
    }

    /*
     * Operator>>= (Shift-Right assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value >>= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value >>= other;
        return *this;
    }

    /*
     * Operator&= (Logical AND assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value &= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value &= other;
        return *this;
    }

    /*
     * Operator|= (Logical OR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value |= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value |= other;
        return *this;
    }

    /*
     * Operator^= (Logical XOR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr Int16& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value ^= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr Int16& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value ^= other;
        return *this;
    }

    /*
     * Operator+ (Addition)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator+(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value + rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Int16 operator+(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value + rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator+(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs + rhs.Value; }

    /*
     * Operator- (Subtraction)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator-(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value - rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Int16 operator-(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value - rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator-(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs - rhs.Value; }

    /*
    * Operator* (Multiplication)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator*(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value * rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Int16 operator*(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value * rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator*(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs * rhs.Value; }

    /*
    * Operator/ (Division)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator/(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value / rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Int16 operator/(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value / rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Int16 operator/(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs / rhs.Value; }

    /*
     * Operator% (Modulo)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator%(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value % other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator%(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value % other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator%(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs % rhs.Value;
    }

    /*
     * Operator& (Logical AND)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator&(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value & other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator&(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value & other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator&(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs & rhs.Value;
    }

    /*
     * Operator| (Logical OR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator|(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value | other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator|(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value | other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator|(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs | rhs.Value;
    }

    /*
     * Operator^ (Logical XOR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator^(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value ^ other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator^(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value ^ other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator^(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs ^ rhs.Value;
    }

    /*
     * Operator<< (Shift-Left)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator<<(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value << other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator<<(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value << other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator<<(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs << rhs.Value;
    }

    /*
     * Operator>> (Shift-Right)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator>>(Int16 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value >> other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator>>(Int16 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value >> other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr Int16 operator>>(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs >> rhs.Value;
    }

    /*
     * Operator== (Equality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value == rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

    /*
     * Operator!= (Inequality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return !(lhs.Value == rhs); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

    /*
     * Operator< (Less Than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value < rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

    /*
     * Operator<= (Less Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value <= rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

    /*
     * Operator> (Greater than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value > rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

    /*
     * Operator>= (Greater Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(Int16 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value >= rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(T const& lhs, Int16 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

    static inline constexpr Int16 MaxValue() noexcept { return static_cast<value_type>(0x7FFF); }
    static inline constexpr Int16 MinValue() noexcept { return static_cast<value_type>(0x8000); }
    static inline constexpr Int16 Zero() noexcept { return static_cast<value_type>(0x0000); }
    static inline constexpr Int16 One() noexcept { return static_cast<value_type>(0x0001); }
    static inline constexpr Int16 NegativeOne() noexcept { return static_cast<value_type>(0xFFFF); }

    Boolean Equals(const Int16& other) const noexcept;
    UInt32 GetHashCode() const noexcept;
    String ToString() const noexcept;
};