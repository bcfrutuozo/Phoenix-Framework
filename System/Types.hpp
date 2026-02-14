#pragma once

// ============================
//  Phoenix Fundamental Types
// ============================

// --- Meta (required for wrappers)
#include "System/Meta/Access.hpp"
#include "System/Meta/WrapperTraits.hpp"
#include "System/Meta/WrapperValue.hpp"
#include "System/Meta/CharTraits.hpp"
#include "System/Meta/PrimitiveToWrapper.hpp"
#include "System/Meta/PromotionPrimitives.hpp"
#include "System/Meta/Promotion.hpp"
#include "System/Meta/HierarchyTraits.hpp"
#include "System/Meta/TypeTraits.hpp"
#include "System/Meta/Language.hpp"

// Object base
#include "System/Types/Fundamentals/Object.hpp"

// --- Fundamental Wrappers
#include "Types/Primitives/Boolean.hpp"
#include "Types/Primitives/Byte.hpp"
#include "Types/Primitives/Char.hpp"
#include "Types/Primitives/CodePoint.hpp"
#include "Types/Primitives/SByte.hpp"

#include "Types/Primitives/Int16.hpp"
#include "Types/Primitives/Int32.hpp"
#include "Types/Primitives/Int64.hpp"

#include "Types/Primitives/UInt16.hpp"
#include "Types/Primitives/UInt32.hpp"
#include "Types/Primitives/UInt64.hpp"

#include "Types/Primitives/Single.hpp"
#include "Types/Primitives/Double.hpp"

#include "Meta/WrapperContract.hpp"

VALIDATE_WRAPPER(Boolean);
VALIDATE_WRAPPER(Byte);
VALIDATE_WRAPPER(Char);
VALIDATE_WRAPPER(CodePoint);
VALIDATE_WRAPPER(SByte);
VALIDATE_WRAPPER(Int16);
VALIDATE_WRAPPER(Int32);
VALIDATE_WRAPPER(Int64);
VALIDATE_WRAPPER(UInt16);
VALIDATE_WRAPPER(UInt32);
VALIDATE_WRAPPER(UInt64);
VALIDATE_WRAPPER(Single);
VALIDATE_WRAPPER(Double);

static_assert(sizeof(Char) == 1);

using u8 = Byte;
using byte = Byte;
using uint8 = Byte;
using c8 = Char;
using c32 = CodePoint;
using f32 = Single;
using f64 = Double;
using i16 = Int16;
using int16 = Int16;
using i32 = Int32;
using int32 = Int32;
using i64 = Int64;
using int64 = Int64;
using i8 = SByte;
using int8 = SByte;
using sbyte = SByte;
using u16 = UInt16;
using uint16 = UInt16;
using u32 = UInt32;
using uint32 = UInt32;
using u64 = UInt64;
using uint64 = UInt64;

template<typename W, typename E>
    requires(is_enum_v<E> && is_promotion_wrapper_v<W>)
constexpr W FromEnum(E e) noexcept
{
    using U = underlying_type_t<E>;
    static_assert(sizeof(U) <= sizeof(typename W::value_type));
    return W(static_cast<U>(e));
}

#include "Types/Fundamentals/Pointer.hpp"
#include "Meta/PointerContract.hpp"

static_assert(sizeof(Pointer) == sizeof(void*));
static_assert(alignof(Pointer) == alignof(void*));

using ptr = Pointer;
using pointer = Pointer;