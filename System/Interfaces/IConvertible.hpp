#pragma once

#include "System/Exceptions.hpp"

template<typename T>
class IConvertible
{
public:

    auto ToBoolean() { return false; }
    auto ToChar() { return '\0'; }
    auto ToSByte() { return 0; }
    auto ToByte() { return 0; }
    auto ToInt16() { return 0; }
    auto ToUInt16() { return 0; }
    auto ToInt32() { return 0; }
    auto ToUInt32() { return 0; }
    auto ToInt64() { return 0; }
    auto ToUInt64() { return 0; }
    auto ToSingle() { return 0; }
    auto ToDouble() { return 0; }
    auto ToString() { return "\0"; }
};