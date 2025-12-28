#ifndef _WIN32

#include "SocketOS.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static SocketError TranslateError(int err) noexcept
{
    switch (err)
    {
    case EWOULDBLOCK:
    case EAGAIN:      return SocketError::WouldBlock;
    case ETIMEDOUT:   return SocketError::Timeout;
    case ECONNRESET: return SocketError::ConnectionReset;
    case ECONNREFUSED:return SocketError::ConnectionRefused;
    case ENOTCONN:    return SocketError::NotConnected;
    case EADDRINUSE:  return SocketError::AddressInUse;
    default:          return SocketError::Unknown;
    }
}

static sockaddr_storage ToSockAddr(const Endpoint& ep, socklen_t& len)
{
    sockaddr_storage ss{};
    Memory::Zero(&ss, sizeof(ss));
    if (ep.Address().IsV4())
    {
        auto* a = reinterpret_cast<sockaddr_in*>(&ss);
        a->sin_family = AF_INET;
        a->sin_port = htons((uint16_t)ep.Port());
        memcpy(&a->sin_addr, ep.Address().GetBytes(), 4);
        len = sizeof(sockaddr_in);
    }
    else
    {
        auto* a = reinterpret_cast<sockaddr_in6*>(&ss);
        a->sin6_family = AF_INET6;
        a->sin6_port = htons((uint16_t)ep.Port());
        memcpy(&a->sin6_addr, ep.Address().GetBytes(), 16);
        len = sizeof(sockaddr_in6);
    }
    return ss;
}

// ------------------------------------------------------------

SocketHandle SocketOS::CreateTcpSocket() noexcept
{
    int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return SocketHandle{ Pointer((uintptr_t)fd) };
}

SocketHandle SocketOS::CreateUdpSocket() noexcept
{
    int fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return SocketHandle{ Pointer((uintptr_t)fd) };
}

void SocketOS::Close(SocketHandle& handle) noexcept
{
    ::close((int)handle.Value.Value());
}

SocketError SocketOS::Connect(SocketHandle& h, const Endpoint& ep) noexcept
{
    socklen_t len;
    auto sa = ToSockAddr(ep, len);
    if (::connect((int)h.Value.Value(), (sockaddr*)&sa, len) != 0)
        return TranslateError(errno);
    return SocketError::None;
}

SocketError SocketOS::Bind(SocketHandle& h, const Endpoint& ep) noexcept
{
    socklen_t len;
    auto sa = ToSockAddr(ep, len);
    if (::bind((int)h.Value.Value(), (sockaddr*)&sa, len) != 0)
        return TranslateError(errno);
    return SocketError::None;
}

SocketError SocketOS::Listen(SocketHandle& h, Int32 backlog) noexcept
{
    if (::listen((int)h.Value.Value(), backlog) != 0)
        return TranslateError(errno);
    return SocketError::None;
}

SocketHandle SocketOS::Accept(SocketHandle& h, SocketError& err) noexcept
{
    int fd = ::accept((int)h.Value.Value(), nullptr, nullptr);
    if (fd < 0)
    {
        err = TranslateError(errno);
        return SocketHandle{};
    }
    err = SocketError::None;
    return SocketHandle{ Pointer((uintptr_t)fd) };
}

// ------------------------------------------------------------

Int32 SocketOS::Send(
    SocketHandle& h, const Byte* d, UInt32 l, SocketError& err) noexcept
{
    int r = ::send((int)h.Value.Value(), d, l, 0);
    if (r < 0)
    {
        err = TranslateError(errno);
        return -1;
    }
    err = SocketError::None;
    return r;
}

Int32 SocketOS::Receive(
    SocketHandle& h, Byte* b, UInt32 c, SocketError& err) noexcept
{
    int r = ::recv((int)h.Value.Value(), b, c, 0);
    if (r < 0)
    {
        err = TranslateError(errno);
        return -1;
    }
    err = SocketError::None;
    return r;
}

// ------------------------------------------------------------

SocketError SocketOS::SetBlocking(
    SocketHandle& h, SocketBase::BlockingMode mode) noexcept
{
    int flags = fcntl((int)h.Value.Value(), F_GETFL, 0);
    if (mode == SocketBase::BlockingMode::NonBlocking)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;

    return fcntl((int)h.Value.Value(), F_SETFL, flags) == 0
        ? SocketError::None
        : TranslateError(errno);
}

Int32 SocketOS::SendTo(
    SocketHandle& h,
    const Endpoint& target,
    const Byte* data,
    UInt32 length,
    SocketError& err) noexcept
{
    socklen_t len;
    sockaddr_storage sa = ToSockAddr(target, len);
    Memory::Zero(&sa, sizeof(sa));

    int r = ::sendto(
        (int)h.Value.Value(),
        data,
        length,
        0,
        reinterpret_cast<sockaddr*>(&sa),
        len);

    if (r < 0)
    {
        err = TranslateError(errno);
        return -1;
    }

    err = SocketError::None;
    return r;
}

Int32 SocketOS::ReceiveFrom(
    SocketHandle& h,
    Endpoint& sender,
    Byte* buffer,
    UInt32 capacity,
    SocketError& err) noexcept
{
    sockaddr_storage sa{};
    socklen_t len = sizeof(sa);
    Memory::Zero(&ss, len);

    int r = ::recvfrom(
        (int)h.Value.Value(),
        buffer,
        capacity,
        0,
        reinterpret_cast<sockaddr*>(&sa),
        &len);

    if (r < 0)
    {
        err = TranslateError(errno);
        return -1;
    }

    // preencher Endpoint do remetente
    if (sa.ss_family == AF_INET)
    {
        auto* a = reinterpret_cast<sockaddr_in*>(&sa);
        IPAddress ip;
        ip = IPAddress::AnyV4(); // base
        memcpy((void*)ip.GetBytes(), &a->sin_addr, 4);

        sender = Endpoint(ip, ntohs(a->sin_port));
    }
    else if (sa.ss_family == AF_INET6)
    {
        auto* a = reinterpret_cast<sockaddr_in6*>(&sa);
        IPAddress ip;
        ip = IPAddress::AnyV6();
        memcpy((void*)ip.GetBytes(), &a->sin6_addr, 16);

        sender = Endpoint(ip, ntohs(a->sin6_port));
    }

    err = SocketError::None;
    return r;
}

SocketError SocketOS::SetRecvTimeout(
    SocketHandle& h,
    UInt32 milliseconds) noexcept
{
    timeval tv;
    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = (milliseconds % 1000) * 1000;

    if (::setsockopt(
        (int)h.Value.Value(),
        SOL_SOCKET,
        SO_RCVTIMEO,
        &tv,
        sizeof(tv)) != 0)
    {
        return TranslateError(errno);
    }

    return SocketError::None;
}

SocketError SocketOS::SetSendTimeout(
    SocketHandle& h,
    UInt32 milliseconds) noexcept
{
    timeval tv;
    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = (milliseconds % 1000) * 1000;

    if (::setsockopt(
        (int)h.Value.Value(),
        SOL_SOCKET,
        SO_SNDTIMEO,
        &tv,
        sizeof(tv)) != 0)
    {
        return TranslateError(errno);
    }

    return SocketError::None;
}

#endif