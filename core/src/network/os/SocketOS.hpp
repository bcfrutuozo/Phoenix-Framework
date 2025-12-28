#pragma once

#include "network/SocketHandle.hpp"
#include "network/SocketError.hpp"
#include "network/Endpoint.hpp"
#include "network/SocketBase.hpp" // BlockingMode
#include "types/Int32.hpp"
#include "types/UInt32.hpp"
#include "types/Byte.hpp"

namespace SocketOS
{
	// ------------------------------------------------------------
	// Creation / destruction
	// ------------------------------------------------------------

	SocketHandle CreateTcpSocket() noexcept;
	SocketHandle CreateUdpSocket() noexcept;

	void Close(SocketHandle& handle) noexcept;

	// ------------------------------------------------------------
	// Connection management
	// ------------------------------------------------------------

	SocketError Connect(SocketHandle& handle, const Endpoint& ep) noexcept;
	SocketError Bind(SocketHandle& handle, const Endpoint& ep) noexcept;
	SocketError Listen(SocketHandle& handle, Int32 backlog) noexcept;
	SocketHandle Accept(SocketHandle& handle, SocketError& err) noexcept;

	// ------------------------------------------------------------
	// I/O
	// ------------------------------------------------------------

	Int32 Send(SocketHandle& handle, const Byte* data, UInt32 length, SocketError& err) noexcept;
	Int32 Receive(SocketHandle& handle, Byte* buffer, UInt32 capacity, SocketError& err) noexcept;
	Int32 SendTo(SocketHandle& handle, const Endpoint& target, const Byte* data, UInt32 length, SocketError& err) noexcept;
	Int32 ReceiveFrom(SocketHandle& handle, Endpoint& sender, Byte* buffer, UInt32 capacity, SocketError& err) noexcept;

	// ------------------------------------------------------------
	// Options
	// ------------------------------------------------------------

	SocketError SetBlocking(SocketHandle& handle, SocketBase::BlockingMode mode) noexcept;
	SocketError SetRecvTimeout(SocketHandle& handle, UInt32 milliseconds) noexcept;
	SocketError SetSendTimeout(SocketHandle& handle, UInt32 milliseconds) noexcept;
}