#pragma once

#include "WrapperTraits.hpp"

// -----------------------------------------
// wrapper_underlying
// -----------------------------------------

template<typename T>
struct wrapper_underlying {
    static_assert(
        is_promotion_wrapper<T>::value,
        "wrapper_underlying<T> requires T to be a wrapper"
        );
    using type = typename T::value_type;
};

template<typename T>
using wrapper_underlying_t = typename wrapper_underlying<T>::type;