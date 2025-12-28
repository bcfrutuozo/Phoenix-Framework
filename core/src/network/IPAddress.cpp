#include "IPAddress.hpp"
#include "system/Memory.hpp"

IPAddress::IPAddress() noexcept
    :
    _family(Family::IPv4)
{
    Memory::Zero(_bytes, 16);
}

IPAddress IPAddress::LoopbackV4() noexcept
{
    IPAddress ip;
    ip._family = Family::IPv4;
    Memory::Zero(ip._bytes, 16);
    ip._bytes[0] = 127;
    ip._bytes[3] = 1;
    return ip;
}

IPAddress IPAddress::LoopbackV6() noexcept
{
    IPAddress ip;
    ip._family = Family::IPv6;
    Memory::Zero(ip._bytes, 16);
    ip._bytes[15] = 1; // ::1  →  0000:0000:0000:0000:0000:0000:0000:0001
    return ip;
}

IPAddress IPAddress::AnyV4() noexcept
{
    IPAddress ip;
    ip._family = Family::IPv4;
    Memory::Zero(ip._bytes, 16);
    return ip;
}

IPAddress IPAddress::AnyV6() noexcept
{
    IPAddress ip;
    ip._family = Family::IPv6;
    Memory::Zero(ip._bytes, 16);
    return ip;
}

Boolean IPAddress::operator==(const IPAddress& other) const noexcept
{
    if (_family != other._family)
        return false;

    Byte len = GetLength();
    for (auto i = 0; i < len; ++i)
        if (_bytes[i] != other._bytes[i])
            return false;

    return true;
}