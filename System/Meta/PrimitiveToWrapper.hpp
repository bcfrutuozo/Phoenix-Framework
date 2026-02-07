#pragma once

#include <cstdint>

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
class Pointer;

template<typename T>
struct primitive_to_wrapper;

template<> struct primitive_to_wrapper<int8_t> { using type = SByte; };
template<> struct primitive_to_wrapper<uint8_t> { using type = Byte; };
template<> struct primitive_to_wrapper<int16_t> { using type = Int16; };
template<> struct primitive_to_wrapper<uint16_t> { using type = UInt16; };
template<> struct primitive_to_wrapper<int32_t> { using type = Int32; };
template<> struct primitive_to_wrapper<uint32_t> { using type = UInt32; };
template<> struct primitive_to_wrapper<int64_t> { using type = Int64; };
template<> struct primitive_to_wrapper<uint64_t> { using type = UInt64; };
template<> struct primitive_to_wrapper<float> { using type = Single; };
template<> struct primitive_to_wrapper<double> { using type = Double; };
template<> struct primitive_to_wrapper<bool> { using type = Boolean; };
template<> struct primitive_to_wrapper<char> { using type = Char; };
template<> struct primitive_to_wrapper<char32_t> { using type = CodePoint; };
template<> struct primitive_to_wrapper<void> { using type = Pointer; };

template<typename T>
using primitive_to_wrapper_t = typename primitive_to_wrapper<T>::type;