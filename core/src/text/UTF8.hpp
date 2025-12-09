#pragma once

#include <cstdlib>

#include "types/Char.hpp"
#include "types/CodePoint.hpp"
#include "types/UInt64.hpp"
#include "types/Boolean.hpp"

namespace UTF8
{
    struct UTF8EncodeResult
    {
        Char Bytes[4];
        UInt64 Length;
    };

    struct UTF8DecodeResult
    {
        CodePoint cp;
        UInt64 Length;
        Boolean IsValid;
    };

    // CodePoint -> UTF-8
    constexpr UTF8EncodeResult encode_utf8(CodePoint cp) noexcept {
        UTF8EncodeResult result{ {Char(), Char(), Char(), Char()}, 0 };

        if (!cp.IsValid()) {
            // invalid codepoint -> zero length, caller must check
            result.Length = 0;
            return result;
        }

        uint32_t v = static_cast<uint32_t>(cp);

        if (v <= 0x7F) {
            result.Bytes[0] = Char(static_cast<uint8_t>(v));
            result.Length = 1;
        }
        else if (v <= 0x7FF) {
            result.Bytes[0] = Char(static_cast<uint8_t>(0xC0 | (v >> 6)));
            result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
            result.Length = 2;
        }
        else if (v <= 0xFFFF) {
            result.Bytes[0] = Char(static_cast<uint8_t>(0xE0 | (v >> 12)));
            result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | ((v >> 6) & 0x3F)));
            result.Bytes[2] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
            result.Length = 3;
        }
        else {
            // até 0x10FFFF
            result.Bytes[0] = Char(static_cast<uint8_t>(0xF0 | (v >> 18)));
            result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | ((v >> 12) & 0x3F)));
            result.Bytes[2] = Char(static_cast<uint8_t>(0x80 | ((v >> 6) & 0x3F)));
            result.Bytes[3] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
            result.Length = 4;
        }

        return result;
    }

    // UTF-8 bytes -> CodePoint
    constexpr UTF8DecodeResult decode_utf8(
        const unsigned char* bytes, size_t maxLen) noexcept {
        UTF8DecodeResult r{ CodePoint(), 0, false };

        if (maxLen == 0) {
            return r;
        }

        uint8_t b0 = static_cast<uint8_t>(bytes[0]);

        // ASCII
        if ((b0 & 0x80) == 0x00) {
            r.cp = CodePoint(static_cast<char32_t>(b0));
            r.Length = 1;
            r.IsValid = true;
            return r;
        }

        // determinando o tamanho esperado
        size_t expected = 0;
        uint32_t cp = 0;

        if ((b0 & 0xE0) == 0xC0) {
            expected = 2;
            cp = (b0 & 0x1F);
        }
        else if ((b0 & 0xF0) == 0xE0) {
            expected = 3;
            cp = (b0 & 0x0F);
        }
        else if ((b0 & 0xF8) == 0xF0) {
            expected = 4;
            cp = (b0 & 0x07);
        }
        else {
            return r; // inválido
        }

        if (maxLen < expected) {
            return r; // não há bytes suficientes
        }

        // lê continuations
        for (size_t i = 1; i < expected; ++i) {
            uint8_t bx = static_cast<uint8_t>(bytes[i]);
            if ((bx & 0xC0) != 0x80) {
                return r; // byte de continuação inválido
            }
            cp = (cp << 6) | (bx & 0x3F);
        }

        CodePoint tmp(static_cast<char32_t>(cp));
        if (!tmp.IsValid()) {
            return r; // codepoint inválido como scalar Unicode
        }

        r.cp = tmp;
        r.Length = expected;
        r.IsValid = true;
        return r;
    }
}