#pragma once

// ============================
//  Phoenix Fundamental Types
// ============================

// --- Meta (required for wrappers)
#include "Meta/BaseDef.hpp"
#include "Meta/TypeTraits.hpp"
#include "Meta/HierarchyTraits.hpp"
#include "Meta/ValueType.hpp"   // facade
#include "Meta/Access.hpp"

// --- Fundamental Wrappers
#include "Types/Boolean.hpp"
#include "Types/Byte.hpp"
#include "Types/Char.hpp"
#include "Types/CodePoint.hpp"
#include "Types/SByte.hpp"

#include "Types/Int16.hpp"
#include "Types/Int32.hpp"
#include "Types/Int64.hpp"

#include "Types/UInt16.hpp"
#include "Types/UInt32.hpp"
#include "Types/UInt64.hpp"

#include "Types/Single.hpp"
#include "Types/Double.hpp"

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

#include "Types/Pointer.hpp"

#include "Meta/PointerContract.hpp"

static_assert(sizeof(Pointer) == sizeof(void*));
static_assert(alignof(Pointer) == alignof(void*));

using ptr = Pointer;
using pointer = Pointer;