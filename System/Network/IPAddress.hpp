#pragma once

#include "System/Types/Byte.hpp"
#include "System/Collections/Array.hpp"

class String;

class IPAddress
{
public:

    enum class Family : uint8_t
    {
        IPv4,
        IPv6
    };

    IPAddress() noexcept;

    static IPAddress LoopbackV4() noexcept;
    static IPAddress LoopbackV6() noexcept;
    static IPAddress AnyV4() noexcept;
    static IPAddress AnyV6() noexcept;

    static IPAddress FromIPv4(Byte a, Byte b, Byte c, Byte d) noexcept;
    static IPAddress FromIPv6(const Byte bytes[16]) noexcept;

    static Boolean TryParse(const String& text, IPAddress& out) noexcept;

    inline constexpr Family GetFamily() const noexcept { return _family; }
    const Byte* GetBytes() const noexcept;

    inline constexpr Byte GetLength() const noexcept { return _family == Family::IPv4 ? 4 : 16; }

    inline constexpr Boolean IsV4() const noexcept { return _family == Family::IPv4; }
    inline constexpr Boolean IsV6() const noexcept { return _family == Family::IPv6; }

    Boolean operator==(const IPAddress& other) const noexcept;
    
    inline constexpr Boolean operator!=(const IPAddress& other) const noexcept { return !(*this == other); }

private:

    Family _family;
    Array<Byte> _bytes;
};