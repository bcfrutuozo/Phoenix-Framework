#pragma once

#include "BaseDef.hpp"
#include "TypeTraits.hpp"

template<typename T>
struct is_promotion_primitive : false_type {
};
template<>
struct is_promotion_primitive<int8_t> : true_type {
};
template<>
struct is_promotion_primitive<int16_t> : true_type {
};
template<>
struct is_promotion_primitive<int32_t> : true_type {
};
template<>
struct is_promotion_primitive<int64_t> : true_type {
};
template<>
struct is_promotion_primitive<char> : true_type {
};
template<>
struct is_promotion_primitive<wchar_t> : true_type {
};
template<>
struct is_promotion_primitive<char8_t> : true_type {
};
template<>
struct is_promotion_primitive<char16_t> : true_type {
};
template<>
struct is_promotion_primitive<char32_t> : true_type {
};
template<>
struct is_promotion_primitive<uint8_t> : true_type {
};
template<>
struct is_promotion_primitive<uint16_t> : true_type {
};
template<>
struct is_promotion_primitive<uint32_t> : true_type {
};
template<>
struct is_promotion_primitive<uint64_t> : true_type {
};
template<>
struct is_promotion_primitive<double> : true_type {
};
template<>
struct is_promotion_primitive<float> : true_type {
};
template<>
struct is_promotion_primitive<bool> : true_type {
};

/*
 * FORWARD DECLARATION OF ALL WRAPPERS
 *
 * This is more than necessary since all wrappers are including  he TypeValue.h
 * So they must be forwarded declared before to handle their implementations
 * on their respective headers
*/
class Boolean;
class Byte;
class Char;
class CodePoint;
class Int16;
class Int32;
class Int64;
class Double;
class Single;
class SByte;
class UInt16;
class UInt32;
class UInt64;

template<typename T>
struct is_promotion_wrapper : false_type {};

template<>
struct is_promotion_wrapper<Boolean> : true_type {};

template<>
struct is_promotion_wrapper<Byte> : true_type {};

template<>
struct is_promotion_wrapper<Char> : true_type {};

template<>
struct is_promotion_wrapper<CodePoint> : true_type {};

template<>
struct is_promotion_wrapper<Int16> : true_type {};

template<>
struct is_promotion_wrapper<Int32> : true_type {};

template<>
struct is_promotion_wrapper<Int64> : true_type {};

template<>
struct is_promotion_wrapper<Double> : true_type {};

template<>
struct is_promotion_wrapper<Single> : true_type {};

template<>
struct is_promotion_wrapper<SByte> : true_type {};

template<>
struct is_promotion_wrapper<UInt16> : true_type {};

template<>
struct is_promotion_wrapper<UInt32> : true_type {};

template<>
struct is_promotion_wrapper<UInt64> : true_type {};

template<typename T>
struct is_wrapper_integral : false_type {};

template<>
struct is_wrapper_integral<Boolean> : true_type {};

template<>
struct is_wrapper_integral<Byte> : true_type {};

template<>
struct is_wrapper_integral<Char> : true_type {};

template<>
struct is_wrapper_integral<CodePoint> : true_type {};

template<>
struct is_wrapper_integral<Int16> : true_type {};

template<>
struct is_wrapper_integral<Int32> : true_type {};

template<>
struct is_wrapper_integral<Int64> : true_type {};

template<>
struct is_wrapper_integral<Double> : false_type {};

template<>
struct is_wrapper_integral<Single> : false_type {};

template<>
struct is_wrapper_integral<SByte> : true_type {};

template<>
struct is_wrapper_integral<UInt16> : true_type {};

template<>
struct is_wrapper_integral<UInt32> : true_type {};

template<>
struct is_wrapper_integral<UInt64> : true_type {};

// ---------------------------------------------------------------------
//  is_single_char
// ---------------------------------------------------------------------
template<typename T>
struct is_single_char : false_type {};

template<> struct is_single_char<Char> : true_type {};
template<> struct is_single_char<char> : true_type {};
template<> struct is_single_char<wchar_t> : true_type {};
template<> struct is_single_char<char8_t> : true_type {};
template<> struct is_single_char<char16_t> : true_type {};
template<> struct is_single_char<char32_t> : true_type {};
template<> struct is_single_char<Byte> : true_type {};
template<> struct is_single_char<CodePoint> : true_type {};

template<typename T>
inline constexpr bool is_single_char_v = is_single_char<T>::value;

// --------------------------------------------------
// primitive_to_wrapper
// --------------------------------------------------

template<typename T>
struct primitive_to_wrapper;

// Inteiros
template<> struct primitive_to_wrapper<int8_t> { using type = SByte; };
template<> struct primitive_to_wrapper<uint8_t> { using type = Byte; };
template<> struct primitive_to_wrapper<int16_t> { using type = Int16; };
template<> struct primitive_to_wrapper<uint16_t> { using type = UInt16; };
template<> struct primitive_to_wrapper<int32_t> { using type = Int32; };
template<> struct primitive_to_wrapper<uint32_t> { using type = UInt32; };
template<> struct primitive_to_wrapper<int64_t> { using type = Int64; };
template<> struct primitive_to_wrapper<uint64_t> { using type = UInt64; };
                  
// Floating       
template<> struct primitive_to_wrapper<float> { using type = Single; };
template<> struct primitive_to_wrapper<double> { using type = Double; };
                  
// Boolean        
template<> struct primitive_to_wrapper<bool> { using type = Boolean; };
                  
// Char           
template<> struct primitive_to_wrapper<char> { using type = Char; };
                  
// CodePoint      
template<> struct primitive_to_wrapper<char32_t> { using type = CodePoint; };

// --------------------------------------------------
// Alias
// --------------------------------------------------
template<typename T>
using primitive_to_wrapper_t = typename primitive_to_wrapper<T>::type;

template<typename T>
constexpr auto Wrapper(T value) noexcept
{
    using WT = primitive_to_wrapper_t<T>;
    return WT(value);
}