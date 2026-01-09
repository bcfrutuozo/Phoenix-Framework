#pragma once

#include "SocketBase.hpp"
#include "Endpoint.hpp"
#include "System/Types/Byte.hpp"
#include "System/Types/UInt32.hpp"

class UdpSocket final : public SocketBase
{
public:
	UdpSocket() noexcept;

	SocketError Bind(const Endpoint& endpoint) noexcept;

	SocketError SetBlocking(BlockingMode mode) noexcept;

	Int32 SendTo(const Endpoint& target, const Byte* data, UInt32 length, SocketError& err) noexcept;
	Int32 ReceiveFrom(Endpoint& sender, Byte* buffer, UInt32 capacity, SocketError& err) noexcept;
};