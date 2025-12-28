#pragma once

#include "meta/BaseDef.hpp"
#include "meta/TypeTraits.hpp"
#include "types/Byte.hpp"

enum class SocketError : uint8_t
{
    None = 0,

    WouldBlock,
    Timeout,
    Interrupted,

    ConnectionReset,
    ConnectionRefused,
    NotConnected,

    AddressInUse,
    AddressNotAvailable,

    NetworkDown,
    NetworkUnreachable,

    InvalidArgument,
    InvalidHandle,
    Unknown
};