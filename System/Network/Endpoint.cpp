#include "Endpoint.hpp"

Endpoint::Endpoint() noexcept
    : _address(), _port(0)
{
}

Endpoint::Endpoint(const IPAddress& address, UInt16 port) noexcept
    : _address(address), _port(port)
{
}

Boolean Endpoint::operator==(const Endpoint& other) const noexcept
{
    return _port == other._port && _address == other._address;
}