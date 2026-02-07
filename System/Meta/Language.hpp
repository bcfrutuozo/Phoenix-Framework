#pragma once

#include <cstdint>

template <typename T, uint32_t N>
constexpr uint32_t CountOf(const T(&)[N]) noexcept
{
    return N;
}

template <typename T>
struct AlignOf
{
    static constexpr uint32_t value = alignof(T);
};

template <typename T>
inline constexpr uint32_t AlignOf_v = AlignOf<T>::value;

template <typename T, typename M>
constexpr uint32_t OffsetOf(M T::* member) noexcept
{
    return static_cast<uint32_t>(
        reinterpret_cast<size_t>(
            &(reinterpret_cast<T const volatile*>(0)->*member)
            )
        );
}