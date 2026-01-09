#include "IPAddress.hpp"
#include "System/String.hpp"
#include "System/Memory.hpp"

IPAddress::IPAddress() noexcept
    :
    _family(Family::IPv4),
    _bytes(16)
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

IPAddress IPAddress::FromIPv4(Byte a, Byte b, Byte c, Byte d) noexcept
{
    IPAddress ip;
    ip._family = Family::IPv4;

    ip._bytes[0] = a;
    ip._bytes[1] = b;
    ip._bytes[2] = c;
    ip._bytes[3] = d;

    return ip;
}

IPAddress IPAddress::FromIPv6(const Byte bytes[16]) noexcept
{
    IPAddress ip;
    ip._family = Family::IPv6;

    for (int i = 0; i < 16; ++i)
        ip._bytes[i] = bytes[i];

    return ip;
}

Boolean IPAddress::TryParse(const String& text, IPAddress& out) noexcept
{
    // ------------------------------------------------------------
    // Tentativa IPv4
    // ------------------------------------------------------------
    {
        Byte parts[4] = {};
        uint32_t partIndex = 0;
        uint32_t value = 0;
        Boolean hasDigit = false;

        for (UInt32 i = 0; i < text.GetLength(); ++i)
        {
            Char c = text[i];

            if (c >= '0' && c <= '9')
            {
                hasDigit = true;
                value = value * 10 + (c - '0');

                if (value > 255)
                    goto ipv4_fail;
            }
            else if (c == '.')
            {
                if (!hasDigit || partIndex >= 3)
                    goto ipv4_fail;

                parts[partIndex++] = static_cast<Byte>(value);
                value = 0;
                hasDigit = false;
            }
            else
            {
                goto ipv4_fail;
            }
        }

        if (partIndex == 3 && hasDigit)
        {
            parts[3] = static_cast<Byte>(value);

            out = IPAddress::FromIPv4(
                parts[0], parts[1], parts[2], parts[3]);
            return true;
        }
    }

ipv4_fail:

    // ------------------------------------------------------------
    // Tentativa IPv6
    // ------------------------------------------------------------
    {
        uint16_t words[8] = {};
        int32_t wordCount = 0;
        int32_t compressIndex = -1;

        uint32_t value = 0;
        uint32_t digits = 0;

        for (uint32_t i = 0; i <= text.GetLength(); ++i)
        {
            char c = (i < text.GetLength()) ? text[i] : (Char)':';

            if ((c >= '0' && c <= '9') ||
                (c >= 'a' && c <= 'f') ||
                (c >= 'A' && c <= 'F'))
            {
                value <<= 4;

                if (c >= '0' && c <= '9') value |= (c - '0');
                else if (c >= 'a')        value |= (c - 'a' + 10);
                else                      value |= (c - 'A' + 10);

                if (++digits > 4)
                    return false;
            }
            else if (c == ':')
            {
                if (digits == 0)
                {
                    if (compressIndex >= 0)
                        return false; // "::" twice

                    compressIndex = wordCount;
                }
                else
                {
                    if (wordCount >= 8)
                        return false;

                    words[wordCount++] = static_cast<uint16_t>(value);
                    value = 0;
                    digits = 0;
                }
            }
            else
            {
                return false;
            }
        }

        if (digits > 0)
        {
            if (wordCount >= 8)
                return false;

            words[wordCount++] = static_cast<uint16_t>(value);
        }

        if (compressIndex >= 0)
        {
            int32_t missing = 8 - wordCount;

            for (int32_t i = wordCount - 1; i >= compressIndex; --i)
                words[i + missing] = words[i];

            for (int32_t i = compressIndex; i < compressIndex + missing; ++i)
                words[i] = 0;

            wordCount = 8;
        }

        if (wordCount != 8)
            return false;

        Byte bytes[16];
        for (int32_t i = 0; i < 8; ++i)
        {
            bytes[i * 2] = static_cast<Byte>(words[i] >> 8);
            bytes[i * 2 + 1] = static_cast<Byte>(words[i] & 0xFF);
        }

        out = IPAddress::FromIPv6(bytes);
        return true;
    }
}

const Byte* IPAddress::GetBytes() const noexcept
{
    return reinterpret_cast<const Byte*>(static_cast<Pointer>(_bytes).Get());
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