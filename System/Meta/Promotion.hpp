#pragma once

#include "BaseDef.hpp"
#include "PrimitiveToWrapper.hpp"
#include "WrapperValue.hpp"

template<typename T>
constexpr auto ToWrapper(T value) noexcept
{
    using WT = primitive_to_wrapper_t<T>;
    return WT(value);
}

template<typename A, typename B>
struct promote_primitive {
    using type = decltype(
        declval<A>() + declval<B>()
        );
};

template<typename A, typename B>
using promote_primitive_t = typename promote_primitive<A, B>::type;

template<typename L, typename R>
using promote_t =
primitive_to_wrapper_t<
    promote_primitive_t<
    wrapper_underlying_t<L>,
    wrapper_underlying_t<R>
    >
>;