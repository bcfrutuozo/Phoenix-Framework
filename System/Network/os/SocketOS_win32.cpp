#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include "SocketOS.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>

namespace SocketOS
{
    static SocketError TranslateError(int err) noexcept
    {
        switch (err)
        {
        case WSAEWOULDBLOCK: return SocketError::WouldBlock;
        case WSAETIMEDOUT: return SocketError::Timeout;
        case WSAECONNRESET: return SocketError::ConnectionReset;
        case WSAECONNREFUSED: return SocketError::ConnectionRefused;
        default: return SocketError::Unknown;
        }
    }

    static sockaddr_storage ToSockAddr(const Endpoint& ep, int& len)
    {
        sockaddr_storage ss{};
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

    SocketHandle CreateTcpSocket() noexcept
    {
        SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return SocketHandle{ Pointer((uintptr_t)s) };
    }

    SocketHandle CreateUdpSocket() noexcept
    {
        SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        return SocketHandle{ Pointer((uintptr_t)s) };
    }

    void SocketOS::Close(SocketHandle& h) noexcept
    {
        ::closesocket((SOCKET)(h.Value));
    }

    SocketError Connect(SocketHandle& handle, const Endpoint& ep) noexcept
    {
        int len;
        sockaddr_storage sa = ToSockAddr(ep, len);

        if (::connect(
            (SOCKET)handle.Value,
            reinterpret_cast<sockaddr*>(&sa),
            len) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }

    SocketError Bind(SocketHandle& handle, const Endpoint& ep) noexcept
    {
        // ------------------------------------------------------------
        // 1. Validar handle
        // ------------------------------------------------------------
        if (!handle.IsValid())
            return SocketError::InvalidHandle;

        SOCKET s = (SOCKET)handle.Value;

        // ------------------------------------------------------------
        // 2. Habilitar reuso de endereço (CRÍTICO para testes)
        // ------------------------------------------------------------
        BOOL reuse = TRUE;
        ::setsockopt(
            s,
            SOL_SOCKET,
            SO_REUSEADDR,
            reinterpret_cast<const char*>(&reuse),
            sizeof(reuse)
        );
        // ⚠️ erro aqui NÃO é fatal — seguimos mesmo assim

        // ------------------------------------------------------------
        // 3. Bind propriamente dito
        // ------------------------------------------------------------
        int len = 0;
        sockaddr_storage sa = ToSockAddr(ep, len);

        if (::bind(
            s,
            reinterpret_cast<sockaddr*>(&sa),
            len) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }

    SocketError Listen(SocketHandle& handle, Int32 backlog) noexcept
    {
        if (::listen(
            (SOCKET)handle.Value,
            backlog) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }

    SocketHandle Accept(SocketHandle& handle, SocketError& err) noexcept
    {
        SOCKET s = ::accept(
            (SOCKET)handle.Value,
            nullptr,
            nullptr);

        if (s == INVALID_SOCKET)
        {
            err = TranslateError(WSAGetLastError());
            return SocketHandle{};
        }

        err = SocketError::None;
        return SocketHandle{ Pointer((uintptr_t)s) };
    }

    Int32 Send(SocketHandle& handle, const Byte* data, UInt32 length, SocketError& err) noexcept
    {
        int r = ::send(
            (SOCKET)handle.Value,
            reinterpret_cast<const char*>(data),
            length,
            0);

        if (r == SOCKET_ERROR)
        {
            err = TranslateError(WSAGetLastError());
            return -1;
        }

        err = SocketError::None;
        return r;
    }

    Int32 Receive(SocketHandle& handle, Byte* buffer, UInt32 capacity, SocketError& err)noexcept
    {
        int r = ::recv(
            (SOCKET)handle.Value,
            reinterpret_cast<char*>(buffer),
            capacity,
            0);

        if (r == SOCKET_ERROR)
        {
            err = TranslateError(WSAGetLastError());
            return -1;
        }

        err = SocketError::None;
        return r;
    }

    Int32 SendTo(SocketHandle& handle, const Endpoint& target, const Byte* data, UInt32 length, SocketError& err) noexcept
    {
        int len;
        sockaddr_storage sa = ToSockAddr(target, len);

        int r = ::sendto(
            (SOCKET)handle.Value,
            reinterpret_cast<const char*>(data),
            length,
            0,
            reinterpret_cast<sockaddr*>(&sa),
            len);

        if (r == SOCKET_ERROR)
        {
            err = TranslateError(WSAGetLastError());
            return -1;
        }

        err = SocketError::None;
        return r;
    }

    Int32 ReceiveFrom(SocketHandle& handle, Endpoint& sender, Byte* buffer, UInt32 capacity, SocketError& err) noexcept
    {
        sockaddr_storage sa{};
        int len = sizeof(sa);

        int r = ::recvfrom(
            (SOCKET)handle.Value,
            reinterpret_cast<char*>(buffer),
            capacity,
            0,
            reinterpret_cast<sockaddr*>(&sa),
            &len);

        if (r == SOCKET_ERROR)
        {
            err = TranslateError(WSAGetLastError());
            return -1;
        }

        if (sa.ss_family == AF_INET)
        {
            auto* a = reinterpret_cast<sockaddr_in*>(&sa);
            IPAddress ip = IPAddress::AnyV4();
            memcpy((void*)ip.GetBytes(), &a->sin_addr, 4);

            sender = Endpoint(ip, ntohs(a->sin_port));
        }
        else if (sa.ss_family == AF_INET6)
        {
            auto* a = reinterpret_cast<sockaddr_in6*>(&sa);
            IPAddress ip = IPAddress::AnyV6();
            memcpy((void*)ip.GetBytes(), &a->sin6_addr, 16);

            sender = Endpoint(ip, ntohs(a->sin6_port));
        }

        err = SocketError::None;
        return r;
    }

    // ------------------------------------------------------------

    SocketError SetBlocking(SocketHandle& handle, SocketBase::BlockingMode mode) noexcept
    {
        // ------------------------------------------------------------
        // 1. Validar handle
        // ------------------------------------------------------------
        if (!handle.IsValid())
            return SocketError::InvalidHandle;

        SOCKET s = (SOCKET)handle.Value;

        // ------------------------------------------------------------
        // 2. Set blocking mode
        // ------------------------------------------------------------
        u_long nonBlocking =
            (mode == SocketBase::BlockingMode::NonBlocking) ? 1 : 0;

        if (::ioctlsocket(
            s,
            FIONBIO,
            &nonBlocking) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }

    SocketError SetRecvTimeout(SocketHandle& handle, UInt32 milliseconds) noexcept
    {
        DWORD ms = static_cast<uint32_t>(milliseconds);

        if (::setsockopt(
            (SOCKET)handle.Value,
            SOL_SOCKET,
            SO_RCVTIMEO,
            reinterpret_cast<const char*>(&ms),
            sizeof(ms)) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }

    SocketError SetSendTimeout(SocketHandle& handle, UInt32 milliseconds) noexcept
    {
        DWORD ms = static_cast<uint32_t>(milliseconds);

        if (::setsockopt(
            (SOCKET)handle.Value,
            SOL_SOCKET,
            SO_SNDTIMEO,
            reinterpret_cast<const char*>(&ms),
            sizeof(ms)) != 0)
        {
            return TranslateError(WSAGetLastError());
        }

        return SocketError::None;
    }
}

#endif