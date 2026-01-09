#pragma once

#include "SocketBase.hpp"
#include "Endpoint.hpp"
#include "System/Types/Byte.hpp"
#include "System/Types/UInt32.hpp"

class TcpSocket final : public SocketBase
{
    friend class TcpListener;

private:

    explicit TcpSocket(SocketHandle h) noexcept
    {
        _handle = h;
    }

public:
    TcpSocket() noexcept;

    SocketError Connect(const Endpoint& endpoint) noexcept;

    Int32 Send(const Byte* data, UInt32 length, SocketError& err) noexcept;
    Int32 Receive(Byte* buffer, UInt32 capacity, SocketError& err) noexcept;

    SocketError SetBlocking(BlockingMode mode) noexcept;

    Boolean IsConnected() const noexcept;
};