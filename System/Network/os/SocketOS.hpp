#pragma once

#include "System/Network/SocketHandle.hpp"
#include "System/Network/SocketError.hpp"
#include "System/Network/Endpoint.hpp"
#include "System/Network/SocketBase.hpp" // BlockingMode

// Not importing System/Types.hpp due to byte redefinition on windows headers on .cpp
#include "System/Types/Primitives/Int32.hpp"
#include "System/Types/Primitives/UInt32.hpp"
#include "System/Types/Primitives/Byte.hpp"

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