#pragma once

// ============================
//  Phoenix Fundamental Types
// ============================

// --- Meta (required for wrappers)
#include "meta/BaseDef.hpp"
#include "meta/TypeTraits.hpp"
#include "meta/HierarchyTraits.hpp"
#include "meta/ValueType.hpp"   // facade
#include "meta/Access.hpp"

// --- Fundamental Wrappers
#include "types/Boolean.hpp"
#include "types/Byte.hpp"
#include "types/Char.hpp"
#include "types/CodePoint.hpp"
#include "types/SByte.hpp"

#include "types/Int16.hpp"
#include "types/Int32.hpp"
#include "types/Int64.hpp"

#include "types/UInt16.hpp"
#include "types/UInt32.hpp"
#include "types/UInt64.hpp"

#include "types/Single.hpp"
#include "types/Double.hpp"

#include "meta/WrapperContract.hpp"

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

typedef Byte u8;
typedef Byte byte;
typedef Byte uint8;
typedef Char c8;
typedef CodePoint c32;
typedef Double f64;
typedef Int16 i16;
typedef Int16 int16;
typedef Int32 i32;
typedef Int32 int32;
typedef Int64 i64;
typedef Int64 int64;
typedef SByte i8;
typedef SByte int8;
typedef UInt16 u16;
typedef UInt16 uint16;
typedef UInt32 u32;
typedef UInt32 uint32;
typedef UInt64 u64;
typedef UInt64 uint64;
typedef String string;
typedef String str;

#include "types/Pointer.hpp"

#include "meta/PointerContract.hpp"

static_assert(sizeof(Pointer) == sizeof(void*));
static_assert(alignof(Pointer) == alignof(void*));

typedef Pointer ptr;
typedef Pointer Void;