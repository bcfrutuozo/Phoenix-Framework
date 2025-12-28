#include "TcpListener.hpp"
#include "network/os/SocketOS.hpp"

TcpListener::TcpListener() noexcept
{
    _handle = SocketOS::CreateTcpSocket();
}

SocketError TcpListener::Bind(const Endpoint& endpoint) noexcept
{
    return SocketOS::Bind(_handle, endpoint);
}

SocketError TcpListener::Listen(Int32 backlog) noexcept
{
    return SocketOS::Listen(_handle, backlog);
}

TcpSocket TcpListener::Accept(SocketError& err) noexcept
{
    SocketHandle h = SocketOS::Accept(_handle, err);

    if (!h.IsValid())
        return TcpSocket{}; // inválido

    return TcpSocket(h);
}