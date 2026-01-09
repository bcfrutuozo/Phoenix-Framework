#include "UDPSocket.hpp"
#include "os/SocketOS.hpp"

UdpSocket::UdpSocket() noexcept
{
    _handle = SocketOS::CreateUdpSocket();
}

SocketError UdpSocket::Bind(const Endpoint& endpoint) noexcept
{
    return SocketOS::Bind(_handle, endpoint);
}

SocketError UdpSocket::SetBlocking(BlockingMode mode) noexcept
{
    if (!IsValid())
        _handle = SocketOS::CreateUdpSocket();
    return SocketOS::SetBlocking(_handle, mode);
}

Int32 UdpSocket::SendTo(
    const Endpoint& target,
    const Byte* data,
    UInt32 length,
    SocketError& err) noexcept
{
    return SocketOS::SendTo(_handle, target, data, length, err);
}

Int32 UdpSocket::ReceiveFrom(
    Endpoint& sender,
    Byte* buffer,
    UInt32 capacity,
    SocketError& err) noexcept
{
    return SocketOS::ReceiveFrom(_handle, sender, buffer, capacity, err);
}