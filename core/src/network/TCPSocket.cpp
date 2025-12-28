#include "TcpSocket.hpp"
#include "network/os/SocketOS.hpp"

TcpSocket::TcpSocket() noexcept
{
    _handle = SocketOS::CreateTcpSocket();
}

SocketError TcpSocket::Connect(const Endpoint& endpoint) noexcept
{
    return SocketOS::Connect(_handle, endpoint);
}

Int32 TcpSocket::Send(const Byte* data, UInt32 length, SocketError& err) noexcept
{
    return SocketOS::Send(_handle, data, length, err);
}

Int32 TcpSocket::Receive(Byte* buffer, UInt32 capacity, SocketError& err) noexcept
{
    return SocketOS::Receive(_handle, buffer, capacity, err);
}

SocketError TcpSocket::SetBlocking(BlockingMode mode) noexcept
{
    if (!IsValid())
        _handle = SocketOS::CreateTcpSocket();
    return SocketOS::SetBlocking(_handle, mode);
}

Boolean TcpSocket::IsConnected() const noexcept
{
    return _handle.IsValid();
}