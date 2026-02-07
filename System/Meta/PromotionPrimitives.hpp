#pragma once
#include "BaseDef.hpp"
#include <cstdint>

// -----------------------------------------
// is_promotion_primitive
// -----------------------------------------

template<typename T>
struct is_promotion_primitive : false_type {};

template<> struct is_promotion_primitive<int8_t> : true_type {};
template<> struct is_promotion_primitive<int16_t> : true_type {};
template<> struct is_promotion_primitive<int32_t> : true_type {};
template<> struct is_promotion_primitive<int64_t> : true_type {};
template<> struct is_promotion_primitive<uint8_t> : true_type {};
template<> struct is_promotion_primitive<uint16_t> : true_type {};
template<> struct is_promotion_primitive<uint32_t> : true_type {};
template<> struct is_promotion_primitive<unsigned long> : true_type {};
template<> struct is_promotion_primitive<uint64_t> : true_type {};
template<> struct is_promotion_primitive<float> : true_type {};
template<> struct is_promotion_primitive<double> : true_type {};
template<> struct is_promotion_primitive<bool> : true_type {};
template<> struct is_promotion_primitive<char> : true_type {};
template<> struct is_promotion_primitive<wchar_t> : true_type {};
template<> struct is_promotion_primitive<char8_t> : true_type {};
template<> struct is_promotion_primitive<char16_t> : true_type {};
template<> struct is_promotion_primitive<char32_t> : true_type {};
template<> struct is_promotion_primitive<void> : true_type {};