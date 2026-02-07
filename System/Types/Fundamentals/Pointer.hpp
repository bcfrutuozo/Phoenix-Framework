#pragma once

#include "System/Meta/ValueType.hpp"
#include "Object.hpp"
#include "System/Types/Primitives/Boolean.hpp"
#include "System/Types/Primitives/Byte.hpp"
#include "System/Types/Primitives/Operators.hpp"
#include "System/Types/Primitives/Int32.hpp"
#include "System/Types/Primitives/UInt64.hpp"
#include "System/Types/Primitives/UInt32.hpp"

class String;

class Pointer final : public Object<Pointer>
{
#define WRAPPER(name) friend class name;
#include "System/Types/Primitives/Wrappers.def"
#undef WRAPPER

	template<typename>
	friend struct WrapperAccess;

public:

#ifdef _M_X64
	using value_type = UInt64;
#else
	using value_type = UInt32;
#endif

private:

	value_type _value;

public:

    inline constexpr Pointer() noexcept : _value(0) {}

    inline constexpr Pointer(decltype(nullptr)) noexcept : _value(0) {}

    explicit inline constexpr Pointer(value_type value) noexcept
        : _value(value) { }

    explicit inline constexpr Pointer(const void* p) noexcept
        : _value(static_cast<value_type>(reinterpret_cast<uintptr_t>(p))) {
    }

    // Implicit to int type (relative to GetValue())

    inline constexpr operator value_type() const noexcept { return _value; }

#ifdef _M_X64
    inline constexpr operator uint64_t() const noexcept { return _value; }
#else
    inline constexpr operator uint32_t() const noexcept { return _value; }
#endif

    inline void* Get() const noexcept
    {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(_value));
    }
    

    inline constexpr Boolean IsNull() const noexcept { return _value == 0; }

    inline constexpr explicit operator bool() const noexcept { return _value != 0; }

    inline constexpr Boolean operator==(const Pointer& other) const noexcept { return _value == other._value; };
    inline constexpr Boolean operator!=(const Pointer& other) const noexcept { return !(*this == other); }

    template<typename T>
    inline static constexpr Pointer AddressOf(T& obj) noexcept { return Pointer(static_cast<void*>(&obj)); }

	Boolean Equals(const Pointer& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;

    static inline constexpr Int32 Size = sizeof(_value);
};