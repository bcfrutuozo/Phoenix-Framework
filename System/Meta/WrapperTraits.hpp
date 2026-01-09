#pragma once

#include "BaseDef.hpp"

// forward declarations
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

// -----------------------------------------
// is_promotion_wrapper
// -----------------------------------------

template<typename T>
struct is_promotion_wrapper : false_type {};

template<> struct is_promotion_wrapper<Boolean> : true_type {};
template<> struct is_promotion_wrapper<Byte> : true_type {};
template<> struct is_promotion_wrapper<Char> : true_type {};
template<> struct is_promotion_wrapper<CodePoint> : true_type {};
template<> struct is_promotion_wrapper<Int16> : true_type {};
template<> struct is_promotion_wrapper<Int32> : true_type {};
template<> struct is_promotion_wrapper<Int64> : true_type {};
template<> struct is_promotion_wrapper<Double> : true_type {};
template<> struct is_promotion_wrapper<Single> : true_type {};
template<> struct is_promotion_wrapper<SByte> : true_type {};
template<> struct is_promotion_wrapper<UInt16> : true_type {};
template<> struct is_promotion_wrapper<UInt32> : true_type {};
template<> struct is_promotion_wrapper<UInt64> : true_type {};

// -----------------------------------------
// is_wrapper_integral
// -----------------------------------------

template<typename T>
struct is_wrapper_integral : false_type {};

template<> struct is_wrapper_integral<Boolean> : true_type {};
template<> struct is_wrapper_integral<Byte> : true_type {};
template<> struct is_wrapper_integral<Char> : true_type {};
template<> struct is_wrapper_integral<CodePoint> : true_type {};
template<> struct is_wrapper_integral<Int16> : true_type {};
template<> struct is_wrapper_integral<Int32> : true_type {};
template<> struct is_wrapper_integral<Int64> : true_type {};
template<> struct is_wrapper_integral<SByte> : true_type {};
template<> struct is_wrapper_integral<UInt16> : true_type {};
template<> struct is_wrapper_integral<UInt32> : true_type {};
template<> struct is_wrapper_integral<UInt64> : true_type {};