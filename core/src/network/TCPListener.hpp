#pragma once

#include "SocketBase.hpp"
#include "Endpoint.hpp"
#include "TcpSocket.hpp"

class TcpListener final : public SocketBase
{
public:
    TcpListener() noexcept;

    SocketError Bind(const Endpoint& endpoint) noexcept;
    SocketError Listen(Int32 backlog = 16) noexcept;

    TcpSocket Accept(SocketError& err) noexcept;
};