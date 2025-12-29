#pragma once

#include <cstdint>

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