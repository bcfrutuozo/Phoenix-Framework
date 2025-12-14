#pragma once

#include "meta/TypeTraits.hpp"
#include "Object.hpp"
#include "types/Boolean.hpp"
#include "types/UInt64.hpp"

class UInt32;
class String;

class CodePoint final : public Object<CodePoint>
{
public:

    using value_type = char32_t;
    value_type Value;

    constexpr CodePoint() : Value(0) {}
    constexpr explicit CodePoint(value_type cp) : Value(cp) {}

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint(T value) noexcept requires(is_promotion_primitive<T>::value) : Value(static_cast<value_type>(value)) {}

    /*
     * Constructor which receives another Wrapper
     * It's implicit because if you pass another wrapper without setting it, it'll cast to primitive
     * by its operator T() function
     */
    template<typename T,
        enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint(T const& wrapper) noexcept
        : Value(static_cast<value_type>(
            static_cast<typename T::value_type>(wrapper))) {
    }

    constexpr CodePoint(CodePoint const&) = default;
    constexpr CodePoint(CodePoint&&) = default;
    constexpr CodePoint& operator=(CodePoint const&) = default;
    constexpr CodePoint& operator=(CodePoint&&) = default;

    /*
    * Operator= (Assignment)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint& operator=(T const& value) noexcept requires(is_promotion_primitive<T>::value) { Value = value; return *this; };

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint& operator=(T const& wrapper) noexcept requires(is_promotion_wrapper<T>::value) { Value = static_cast<value_type>(wrapper.Value); return *this; };

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
    constexpr CodePoint const& operator+() const noexcept {
        return *this;
    }

    /*
     * Operator~ (One's Complement)
     */
    constexpr CodePoint operator~() const noexcept {
        return CodePoint(~Value);
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
    constexpr CodePoint& operator++() noexcept {
        ++Value;
        return *this;
    }

    /*
     * Operator++ (Postfix increment)
     */
    constexpr CodePoint operator++(int) noexcept {
        return CodePoint(Value++);
    }

    /*
     * Operator-- (Prefix decrement)
     */
    constexpr CodePoint& operator--() noexcept {
        --Value;
        return *this;
    }

    /*
     * Operator-- (Postfix decrement)
     */
    constexpr CodePoint operator--(int) noexcept {
        return CodePoint(Value--);
    }

    /*
     * Operator+= (Addition assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint& operator+=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value += other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint& operator+=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value += other.Value;
        return *this;
    }

    /*
     * Operator-= (Subtraction assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint& operator-=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value -= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint& operator-=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value -= other.Value;
        return *this;
    }

    /*
     * Operator*= (Multiplication assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint& operator *=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value *= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint& operator*=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value *= other.Value;
        return *this;
    }

    /*
     * Operator/= (Division assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    constexpr CodePoint& operator /=(T const& other) noexcept requires(is_promotion_primitive<T>::value) {
        Value /= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    constexpr CodePoint& operator/=(T const& other) noexcept requires(is_promotion_wrapper<T>::value) {
        Value /= other.Value;
        return *this;
    }

    /*
     * Operator%= (Modulo assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator %=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value %= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator %=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value %= other.Value;
        return *this;
    }

    /*
     * Operator<<= (Shift-Left assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator <<=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value <<= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator <<=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value <<= other.Value;
        return *this;
    }

    /*
     * Operator>>= (Shift-Right assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator >>=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value >>= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator >>=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value >>= other.Value;
        return *this;
    }

    /*
     * Operator&= (Logical AND assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator &=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value &= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator &=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value &= other.Value;
        return *this;
    }

    /*
     * Operator|= (Logical OR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator |=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value |= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator |=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value |= other.Value;
        return *this;
    }

    /*
     * Operator^= (Logical XOR assignment)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    constexpr CodePoint& operator ^=(T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        Value ^= other;
        return *this;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    constexpr CodePoint& operator ^=(T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        Value ^= other.Value;
        return *this;
    }

    /*
     * Operator+ (Addition)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator+(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value + rhs; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr auto operator+(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value + rhs.Value; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator+(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs + rhs.Value; return Wrapper(res); }

    /*
     * Operator- (Subtraction)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator-(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value - rhs; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr auto operator-(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value - rhs.Value; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator-(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs - rhs.Value; return Wrapper(res); }

    /*
    * Operator* (Multiplication)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator*(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value * rhs; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr auto operator*(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value * rhs.Value; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator*(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs * rhs.Value; return Wrapper(res); }

    /*
    * Operator/ (Division)
    */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator/(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs.Value / rhs; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr auto operator/(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { auto res = lhs.Value / rhs.Value; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr auto operator/(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { auto res = lhs / rhs.Value; return Wrapper(res); }

    /*
     * Operator% (Modulo)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator%(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs.Value % other; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr auto operator%(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) { auto res = lhs.Value % other.Value; return Wrapper(res); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator%(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) { auto res = lhs % rhs.Value; return Wrapper(res); }

    /*
     * Operator& (Logical AND)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator&(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value & other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator&(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value & other.Value;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator&(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs & rhs.Value;
    }

    /*
     * Operator| (Logical OR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator|(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value | other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator|(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value | other.Value;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator|(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs | rhs.Value;
    }

    /*
     * Operator^ (Logical XOR)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator^(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        return lhs.Value ^ other;
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator^(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        return lhs.Value ^ other.Value;
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr CodePoint operator^(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        return lhs ^ rhs.Value;
    }

    /*
     * Operator<< (Shift-Left)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator<<(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        using R = decltype(lhs.Value << other);
        return Wrapper(R(lhs.Value << other));
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr auto operator<<(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        using R = decltype(lhs.Value << other.Value);
        return Wrapper(R(lhs.Value << other.Value));
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator<<(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        using R = decltype(lhs.Value >> rhs);
        return Wrapper(R(lhs.Value >> rhs));
    }

    /*
     * Operator>> (Shift-Right)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator>>(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value) {
        auto res = lhs.Value >> other;
        return Wrapper(res);
    }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value, bool> = true>
    friend inline constexpr auto operator>>(CodePoint const& lhs, T const& other) noexcept requires(is_promotion_wrapper<T>::value&& is_wrapper_integral<T>::value) {
        using R = decltype(lhs.Value >> other.Value);
        return Wrapper(R(lhs.Value >> other.Value));
    }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value&& is_integral<T>::value, bool> = true>
    friend inline constexpr auto operator>>(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value&& is_integral<T>::value)
    {
        using R = decltype(lhs >> rhs.Value);
        return Wrapper(R(lhs >> rhs.Value));
    }

    /*
     * Operator== (Equality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value == rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value == rhs.Value; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator==(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs == rhs.Value; }

    /*
     * Operator!= (Inequality)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs.Value == rhs); }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return !(lhs.Value == rhs.Value); }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator!=(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return !(lhs == rhs.Value); }

    /*
     * Operator< (Less Than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value < rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value < rhs.Value; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs < rhs.Value; }

    /*
     * Operator<= (Less Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value <= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value <= rhs.Value; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator<=(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs <= rhs.Value; }

    /*
     * Operator> (Greater than)
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value > rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value > rhs.Value; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs > rhs.Value; }

    /*
     * Operator>= (Greater Than or Equal to )
     */
    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs.Value >= rhs; }

    template<typename T, enable_if_t<is_promotion_wrapper<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(CodePoint const& lhs, T const& rhs) noexcept requires(is_promotion_wrapper<T>::value) { return lhs.Value >= rhs.Value; }

    template<typename T, enable_if_t<is_promotion_primitive<T>::value, bool> = true>
    friend inline constexpr Boolean operator>=(T const& lhs, CodePoint const& rhs) noexcept requires(is_promotion_primitive<T>::value) { return lhs >= rhs.Value; }

    constexpr Boolean IsValid() const noexcept { return Value <= 0x10FFFF && !(Value >= 0xD800 && Value <= 0xDFFF); }
    constexpr Boolean IsASCII() const noexcept { return Value <= 0x7F; }

    constexpr UInt64 ByteCount() const noexcept {
        if (Value <= 0x7F) return 1;           // ASCII
        else if (Value <= 0x7FF) return 2;     // 2 bytes UTF-8
        else if (Value <= 0xFFFF) return 3;    // 3 bytes UTF-8
        else return 4;                         // 4 bytes UTF-8 (até 0x10FFFF)
    }

    // Unicode non-character range
    constexpr Boolean IsNonCharacter() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0xFDD0 && v <= 0xFDEF) ||
            (v & 0xFFFE) == 0xFFFE; // U+xxFFFE and U+xxFFFF
    }

    constexpr Boolean IsSurrogate() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0xD800 && v <= 0xDFFF);
    }

    constexpr Boolean IsPrivateUse() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return  (v >= 0xE000 && v <= 0xF8FF) ||  // BMP PUA
            (v >= 0xF0000 && v <= 0xFFFFD) ||  // Plane 15
            (v >= 0x100000 && v <= 0x10FFFD);  // Plane 16
    }

    constexpr Boolean IsControl() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v <= 0x1F) || (v >= 0x7F && v <= 0x9F);
    }

    // Used for quick classification before CCC lookup
    constexpr Boolean IsCombiningMark() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0x0300 && v <= 0x036F) ||          // Combining Diacritical Marks
            (v >= 0x1AB0 && v <= 0x1AFF) ||          // Combining Diacritical Marks Extended
            (v >= 0x1DC0 && v <= 0x1DFF) ||          // Combining Diacritical Marks Supplement
            (v >= 0x20D0 && v <= 0x20FF) ||          // Combining Diacritical Marks for Symbols
            (v >= 0xFE20 && v <= 0xFE2F);            // Combining Half Marks
    }

    // Hangul decomposition helpers
    constexpr Boolean IsHangulSyllable() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0xAC00 && v <= 0xD7A3);
    }

    constexpr Boolean IsHangulL() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0x1100 && v <= 0x1112);
    }

    constexpr Boolean IsHangulV() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0x1161 && v <= 0x1175);
    }

    constexpr Boolean IsHangulT() const noexcept {
        uint32_t v = static_cast<uint32_t>(Value);
        return (v >= 0x11A7 && v <= 0x11C2);
    }

    constexpr Boolean IsBMP() const noexcept {
        return Value <= 0xFFFF;
    }

    constexpr Boolean IsWhiteSpace() const noexcept
    {
        uint32_t v = static_cast<uint32_t>(Value);
        // ASCII
        if (v == 0x20 || (v >= 0x09 && v <= 0x0D)) return true;
        if (v == 0x0085) return true;           // NEL
        if (v == 0x00A0) return true;           // NO-BREAK SPACE
        if (v >= 0x2000 && v <= 0x200A) return true;
        if (v == 0x2028 || v == 0x2029) return true;
        if (v == 0x202F) return true;
        if (v == 0x205F) return true;
        if (v == 0x3000) return true;
        return false;
    }

    constexpr uint32_t Plane() const noexcept {
        return static_cast<uint32_t>(Value) >> 16;
    }

    Boolean Equals(const CodePoint& other) const noexcept;
    UInt32 GetHashCode() const noexcept;
    String ToString() const noexcept;
};

typedef CodePoint c32;