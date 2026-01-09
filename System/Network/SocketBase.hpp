#pragma once

#include "SocketHandle.hpp"
#include "SocketError.hpp"

#include "System/Types/UInt32.hpp"

// RAII common base (DO NOT INSTANTIATE IT DIRECTLY!)
class SocketBase
{
public:

    enum class BlockingMode : uint8_t
    {
        Blocking,
        NonBlocking
    };

    SocketBase() noexcept;
    ~SocketBase() noexcept;

    SocketBase(SocketBase&&) noexcept;
    SocketBase& operator=(SocketBase&&) noexcept;

    SocketBase(const SocketBase&) = delete;
    SocketBase& operator=(const SocketBase&) = delete;

    Boolean IsValid() const noexcept;
    SocketHandle Handle() const noexcept;

    void Close() noexcept;

    SocketError SetBlocking(BlockingMode mode) noexcept;
    SocketError SetRecvTimeout(UInt32 milliseconds) noexcept;
    SocketError SetSendTimeout(UInt32 milliseconds) noexcept;

protected:
    SocketHandle _handle;
};