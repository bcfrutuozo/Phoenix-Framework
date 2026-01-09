#pragma once

#include "BaseDef.hpp"
#include "WrapperTraits.hpp"

// forward declarations
class Char;
class CodePoint;
class Byte;

// -----------------------------------------
// is_single_char
// -----------------------------------------

template<typename T>
struct is_single_char : false_type {};

// Wrappers
template<> struct is_single_char<Char> : true_type {};
template<> struct is_single_char<CodePoint> : true_type {};
template<> struct is_single_char<Byte> : true_type {};

// Primitivos
template<> struct is_single_char<char> : true_type {};
template<> struct is_single_char<wchar_t> : true_type {};
template<> struct is_single_char<char8_t> : true_type {};
template<> struct is_single_char<char16_t> : true_type {};
template<> struct is_single_char<char32_t> : true_type {};

template<typename T>
inline constexpr bool is_single_char_v = is_single_char<T>::value;