#pragma once

#include "IPAddress.hpp"
#include "System/Types/UInt16.hpp"

class Endpoint
{
public:

	Endpoint() noexcept;
	Endpoint(const IPAddress& address, UInt16 port) noexcept;

	inline constexpr const IPAddress& Address() const noexcept { return _address; }
	inline constexpr UInt16 Port() const noexcept { return _port; }

	Boolean operator==(const Endpoint&) const noexcept;
	inline constexpr Boolean operator!=(const Endpoint& other) const noexcept { return !(*this == other); }

private:
	IPAddress _address;
	UInt16    _port;
};