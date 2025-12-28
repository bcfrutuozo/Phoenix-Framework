#include "catch_amalgamated.hpp"

// ------------------------------------------------------------
// Network headers
// ------------------------------------------------------------
#include "network/IPAddress.hpp"
#include "network/Endpoint.hpp"
#include "network/SocketHandle.hpp"
#include "network/SocketBase.hpp"
#include "network/TcpSocket.hpp"
#include "network/TcpListener.hpp"
#include "network/UdpSocket.hpp"

// ------------------------------------------------------------
// Primitive types
// ------------------------------------------------------------
#include "types/UInt16.hpp"
#include "types/Byte.hpp"

// ============================================================
// IPAddress
// ============================================================

TEST_CASE("IPAddress: AnyV4 / AnyV6")
{
    IPAddress v4 = IPAddress::AnyV4();
    IPAddress v6 = IPAddress::AnyV6();

    REQUIRE(v4.IsV4());
    REQUIRE(v6.IsV6());

    for (uint32_t i = 0; i < v4.GetLength(); ++i)
        REQUIRE(v4.GetBytes()[i] == 0);

    for (uint32_t i = 0; i < v6.GetLength(); ++i)
        REQUIRE(v6.GetBytes()[i] == 0);
}

TEST_CASE("IPAddress: LoopbackV4 / LoopbackV6")
{
    IPAddress v4 = IPAddress::LoopbackV4();
    IPAddress v6 = IPAddress::LoopbackV6();

    REQUIRE(v4.IsV4());
    REQUIRE(v6.IsV6());

    REQUIRE(v4.GetBytes()[0] == 127);
    REQUIRE(v4.GetBytes()[3] == 1);

    for (uint32_t i = 0; i < 15; ++i)
        REQUIRE(v6.GetBytes()[i] == 0);

    REQUIRE(v6.GetBytes()[15] == 1);
}

// ============================================================
// Endpoint
// ============================================================

TEST_CASE("Endpoint: Equality and inequality")
{
    Endpoint a(IPAddress::LoopbackV4(), UInt16(8080));
    Endpoint b(IPAddress::LoopbackV4(), UInt16(8080));
    Endpoint c(IPAddress::LoopbackV4(), UInt16(8081));

    REQUIRE(a == b);
    REQUIRE(a != c);
}

// ============================================================
// SocketHandle
// ============================================================

TEST_CASE("SocketHandle: Invalid by default")
{
    SocketHandle h;
    REQUIRE(!h.IsValid());

    h.Reset();
    REQUIRE(!h.IsValid());
}

// ============================================================
// TCP (integration / loopback)
// ============================================================

TEST_CASE("[integration][tcp] Bind / Listen / Accept")
{
    TcpListener listener;
    SocketError err;

    // Porta fixa por enquanto (ajustar depois para dinâmica)
    Endpoint ep(IPAddress::LoopbackV4(), UInt16(9001));

    REQUIRE(listener.Bind(ep) == SocketError::None);
    REQUIRE(listener.Listen() == SocketError::None);

    TcpSocket client;
    REQUIRE(client.Connect(ep) == SocketError::None);

    TcpSocket server = listener.Accept(err);
    REQUIRE(err == SocketError::None);
    REQUIRE(server.IsValid());
}

TEST_CASE("[integration][tcp] Send and Receive")
{
    TcpListener listener;
    SocketError err;

    Endpoint ep(IPAddress::LoopbackV4(), UInt16(9002));

    REQUIRE(listener.Bind(ep) == SocketError::None);
    REQUIRE(listener.Listen() == SocketError::None);

    TcpSocket client;
    REQUIRE(client.Connect(ep) == SocketError::None);

    TcpSocket server = listener.Accept(err);
    REQUIRE(err == SocketError::None);

    Byte msg[] = { 'O', 'K' };
    SocketError sendErr;

    REQUIRE(client.Send(msg, 2, sendErr) == 2);
    REQUIRE(sendErr == SocketError::None);

    Byte buf[8];
    SocketError recvErr;

    REQUIRE(server.Receive(buf, 8, recvErr) == 2);
    REQUIRE(recvErr == SocketError::None);

    REQUIRE(buf[0] == 'O');
    REQUIRE(buf[1] == 'K');
}

// ============================================================
// UDP (integration / loopback)
// ============================================================

TEST_CASE("[integration][udp] SendTo / ReceiveFrom")
{
    UdpSocket receiver;
    UdpSocket sender;
    SocketError err;

    Endpoint recvEp(IPAddress::LoopbackV4(), UInt16(9003));
    REQUIRE(receiver.Bind(recvEp) == SocketError::None);

    Byte msg[] = { 1, 2, 3, 4 };
    SocketError sendErr;

    REQUIRE(sender.SendTo(
        recvEp,
        msg,
        4,
        sendErr
    ) == 4);

    REQUIRE(sendErr == SocketError::None);

    Byte buf[16];
    Endpoint from;
    SocketError recvErr;

    REQUIRE(receiver.ReceiveFrom(
        from,
        buf,
        16,
        recvErr
    ) == 4);

    REQUIRE(recvErr == SocketError::None);
    REQUIRE(buf[0] == 1);
    REQUIRE(buf[3] == 4);
}

// ============================================================
// SocketBase options
// ============================================================

TEST_CASE("SocketBase: Blocking and NonBlocking modes")
{
    TcpSocket s;

    REQUIRE(
        s.SetBlocking(SocketBase::BlockingMode::NonBlocking)
        == SocketError::None
    );

    REQUIRE(
        s.SetBlocking(SocketBase::BlockingMode::Blocking)
        == SocketError::None
    );
}