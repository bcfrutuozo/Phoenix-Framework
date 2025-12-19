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

template struct WrapperContract<Boolean>;
template struct WrapperContract<Byte>;
template struct WrapperContract<Char>;
template struct WrapperContract<CodePoint>;
template struct WrapperContract<SByte>;

template struct WrapperContract<Int16>;
template struct WrapperContract<Int32>;
template struct WrapperContract<Int64>;

template struct WrapperContract<UInt16>;
template struct WrapperContract<UInt32>;
template struct WrapperContract<UInt64>;

template struct WrapperContract<Single>;
template struct WrapperContract<Double>;

static_assert(sizeof(Char) == 1);

typedef Byte u8;
typedef Char c8;
typedef CodePoint c32;
typedef Double f64;
typedef Int16 i16;
typedef Int32 i32;
typedef Int64 i64;
typedef SByte i8;
typedef UInt16 u16;
typedef UInt32 u32;
typedef UInt64 u64;