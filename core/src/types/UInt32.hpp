#pragma once

#include "meta/ValueType.hpp"
#include "Object.hpp"
#include "Boolean.hpp"

class String;

class UInt32 final : public Object<UInt32>
{
private:

    using value_type = uint32_t;
    value_type Value;

public:

    constexpr UInt32() : Value() {};

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32(T value) noexcept requires(is_promotion_primitive<T>::value) : Value((value)) {}

    /*
     * Constructor which receives another Wrapper
     * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
     * by its operator T() function
     */
    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr explicit UInt32(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) : Value(wrapper) {}

    constexpr UInt32(UInt32 const&) = default;

    constexpr UInt32(UInt32&&) = default;

    constexpr UInt32& operator=(UInt32 const&) = default;

    constexpr UInt32& operator=(UInt32&&) = default;

    /*
     * Operator= (Assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr UInt32& operator=(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) { Value = wrapper; return *this; };

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
    constexpr UInt32 const& operator+() const noexcept {
        return *this;
    }

    /*
     * Operator~ (One's Complement)
     */
    constexpr UInt32 operator~() const noexcept {
        return UInt32(~Value);
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
    constexpr UInt32& operator++() noexcept {
        ++Value;
        return *this;
    }

    /*
     * Operator++ (Postfix increment)
     */
    constexpr UInt32 operator++(int) noexcept {
        return UInt32(Value++);
    }

    /*
     * Operator-- (Prefix decrement)
     */
    constexpr UInt32& operator--() noexcept {
        --Value;
        return *this;
    }

    /*
     * Operator-- (Postfix decrement)
     */
    constexpr UInt32 operator--(int) noexcept {
        return UInt32(Value--);
    }

    /*
     * Operator+= (Addition assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value += other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr UInt32& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value += other;
        return *this;
    }


    /*
     * Operator-= (Subtraction assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value -= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr UInt32& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value -= other;
        return *this;
    }

    /*
     * Operator*= (Multiplication assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value *= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr UInt32& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value *= other;
        return *this;
    }

    /*
     * Operator/= (Division assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr UInt32& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value /= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr UInt32& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value /= other;
        return *this;
    }

    /*
     * Operator%= (Modulo assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value %= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value %= other;
        return *this;
    }

    /*
     * Operator<<= (Shift-Left assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value <<= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value <<= other;
        return *this;
    }

    /*
     * Operator>>= (Shift-Right assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value >>= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value >>= other;
        return *this;
    }

    /*
     * Operator&= (Logical AND assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value &= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value &= other;
        return *this;
    }

    /*
     * Operator|= (Logical OR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value |= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value |= other;
        return *this;
    }

    /*
     * Operator^= (Logical XOR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr UInt32& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value ^= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr UInt32& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value ^= other;
        return *this;
    }

    /*
     * Operator+ (Addition)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator+(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value + rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr UInt32 operator+(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value + rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator+(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs + rhs.Value; }

    /*
     * Operator- (Subtraction)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator-(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value - rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr UInt32 operator-(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value - rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator-(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs - rhs.Value; }

    /*
    * Operator* (Multiplication)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator*(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value * rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr UInt32 operator*(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value * rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator*(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs * rhs.Value; }

    /*
    * Operator/ (Division)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator/(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value / rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr UInt32 operator/(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value / rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr UInt32 operator/(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs / rhs.Value; }

    /*
     * Operator% (Modulo)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator%(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value % other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator%(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value % other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator%(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs % rhs.Value;
    }

    /*
     * Operator& (Logical AND)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator&(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value & other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator&(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value & other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator&(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs & rhs.Value;
    }

    /*
     * Operator| (Logical OR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator|(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value | other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator|(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value | other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator|(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs | rhs.Value;
    }

    /*
     * Operator^ (Logical XOR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator^(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value ^ other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator^(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value ^ other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator^(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs ^ rhs.Value;
    }

    /*
     * Operator<< (Shift-Left)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator<<(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value << other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator<<(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value << other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator<<(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs << rhs.Value;
    }

    /*
     * Operator>> (Shift-Right)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator>>(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value >> other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator>>(UInt32 const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value >> other;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr UInt32 operator>>(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs >> rhs.Value;
    }

    /*
     * Operator== (Equality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value == rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

    /*
     * Operator!= (Inequality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return !(lhs.Value == rhs); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

    /*
     * Operator< (Less Than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value < rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

    /*
     * Operator<= (Less Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value <= rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

    /*
     * Operator> (Greater than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value > rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

    /*
     * Operator>= (Greater Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(UInt32 const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value >= rhs; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(T const& lhs, UInt32 const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

    static inline constexpr UInt32 MaxValue() noexcept { return static_cast<value_type>(0xFFFFFFFF); }
    static inline constexpr UInt32 MinValue() noexcept { return static_cast<value_type>(0x00000000); }
    static inline constexpr UInt32 Zero() noexcept { return static_cast<value_type>(0x00000000); }
    static inline constexpr UInt32 One() noexcept { return static_cast<value_type>(0x00000001); }

    Boolean Equals(const UInt32& other) const noexcept;
    UInt32 GetHashCode() const noexcept;
    String ToString() const noexcept;
};