#pragma once
#include "TextWriter.hpp"

#include "System/Types.hpp"

#include <cstdio>
#include <cstring>

namespace StreamOperators {

    template <typename T>
    inline std::enable_if_t<
        std::is_base_of_v<Object<T>, T>,
        TextWriter&
    >
        operator<<(TextWriter& w, const T& obj) noexcept
    {
        return w << obj.ToString();
    }

    // -----------------------------------------------------------------------------
    //  bool
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, bool b) noexcept {
        auto s = Boolean(b).ToString();
        w.Write(s);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  char*  (UTF-8 literal)
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const char* s) noexcept {
        if (s) w.Write(s);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  char
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, char c) noexcept {
        w.WriteBytes(&c, 1);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  int8_t e uint8_t devem ser tratados como inteiros, NÃO como char
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, uint8_t v) noexcept {
        char buf[8];
        int n = std::snprintf(buf, sizeof(buf), "%u", (unsigned)v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    inline TextWriter& operator<<(TextWriter& w, int8_t v) noexcept {
        char buf[8];
        int n = std::snprintf(buf, sizeof(buf), "%d", (int)v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  boolean
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, Boolean v) noexcept {
        if (v)
            w.Write("true");
        else
            w.Write("false");
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Byte
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const Byte& b) noexcept {
        unsigned v = static_cast<unsigned char>(b);
        char buf[8];
        int n = std::snprintf(buf, sizeof(buf), "%u", v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Char
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const Char& c) noexcept {
        w.WriteChar(c);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Char* – string UTF-8 composta de Char
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const Char* s) noexcept {
        w.Write(s);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  String – string UTF-8 da framework
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const String& s) noexcept {
        w.Write(s);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  CodePoint
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const CodePoint& cp) noexcept {
        uint32_t v = (uint32_t)cp;
        char buf[16];
        int n = std::snprintf(buf, sizeof(buf), "U+%04X", v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Unsigned Wrappers (UInt16, UInt32, UInt64)
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const UInt16& v) noexcept {
        uint16_t value = static_cast<uint16_t>(v);
        char buf[16];
        int n = std::snprintf(buf, sizeof(buf), "%u", static_cast<unsigned>(value));
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    inline TextWriter& operator<<(TextWriter& w, const UInt32& v) noexcept {
        uint32_t value = static_cast<uint32_t>(v);
        char buf[32];
        int n = std::snprintf(buf, sizeof(buf), "%u", static_cast<unsigned long>(value));
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    inline TextWriter& operator<<(TextWriter& w, const UInt64& v) noexcept {
        uint64_t value = static_cast<uint64_t>(v);
        char buf[32];
        int n = std::snprintf(buf, sizeof(buf), "%llu", static_cast<unsigned long long>(value));
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  float / double / long double
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, float v) noexcept {
        char buf[64];
        int n = std::snprintf(buf, sizeof(buf), "%g", (double)v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    inline TextWriter& operator<<(TextWriter& w, double v) noexcept {
        char buf[64];
        int n = std::snprintf(buf, sizeof(buf), "%g", v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    inline TextWriter& operator<<(TextWriter& w, long double v) noexcept {
        char buf[128];
        int n = std::snprintf(buf, sizeof(buf), "%Lg", v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Inteiros assinados e não-assinados genéricos
    // -----------------------------------------------------------------------------
    template<typename T>
    inline std::enable_if_t<
        is_integral_v<T>&&
        is_signed_v<T> &&
        !is_same_v<T, char> &&
        !is_same_v<T, int8_t>,
        TextWriter&
    >
        operator<<(TextWriter& w, T v) noexcept {
        char buf[64];
        int n = std::snprintf(buf, sizeof(buf), "%lld", (long long)v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    template<typename T>
    inline std::enable_if_t<
        is_integral_v<T>&&
        is_unsigned_v<T> &&
        !is_same_v<T, unsigned char> &&
        !is_same_v<T, uint8_t>,
        TextWriter&
    >
        operator<<(TextWriter& w, T v) noexcept {
        char buf[64];
        int n = std::snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

    // -----------------------------------------------------------------------------
    //  Ponteiros
    // -----------------------------------------------------------------------------
    inline TextWriter& operator<<(TextWriter& w, const void* p) noexcept {
        char buf[32];
        int n = std::snprintf(buf, sizeof(buf), "%p", p);
        if (n > 0) w.WriteBytes(buf, n);
        return w;
    }

} // namespace StreamOperators