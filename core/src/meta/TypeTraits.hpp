#pragma once

#include <cstdint>
#include "BaseDef.hpp"     // usa true_type/false_type/enable_if

// ---------------------------------------------------------------------
//  is_integral
// ---------------------------------------------------------------------

template<typename T>
struct is_integral : false_type {};

template<> struct is_integral<bool> : true_type {};
template<> struct is_integral<char> : true_type {};
template<> struct is_integral<signed char> : true_type {};
template<> struct is_integral<unsigned char> : true_type {};
template<> struct is_integral<short> : true_type {};
template<> struct is_integral<unsigned short> : true_type {};
template<> struct is_integral<int> : true_type {};
template<> struct is_integral<unsigned int> : true_type {};
template<> struct is_integral<long> : true_type {};
template<> struct is_integral<unsigned long> : true_type {};
template<> struct is_integral<long long> : true_type {};
template<> struct is_integral<unsigned long long> : true_type {};

template<typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

// ---------------------------------------------------------------------
//  is_signed
// ---------------------------------------------------------------------
template<typename T>
struct is_signed : false_type {};

template<> struct is_signed<signed char> : true_type {};
template<> struct is_signed<short> : true_type {};
template<> struct is_signed<int> : true_type {};
template<> struct is_signed<long> : true_type {};
template<> struct is_signed<long long> : true_type {};
template<> struct is_signed<char> : true_type {}; // dependendo do compilador, char pode ser signed

template<typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;

// ---------------------------------------------------------------------
//  is_unsigned
// ---------------------------------------------------------------------
template<typename T>
struct is_unsigned : false_type {};

template<> struct is_unsigned<unsigned char> : true_type {};
template<> struct is_unsigned<unsigned short> : true_type {};
template<> struct is_unsigned<unsigned int> : true_type {};
template<> struct is_unsigned<unsigned long> : true_type {};
template<> struct is_unsigned<unsigned long long> : true_type {};

template<typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

// ---------------------------------------------------------------------
//  is_floating_point
// ---------------------------------------------------------------------
template<typename T>
struct is_floating_point : false_type {};

template<> struct is_floating_point<float> : true_type {};
template<> struct is_floating_point<double> : true_type {};
template<> struct is_floating_point<long double> : true_type {};

template<typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;