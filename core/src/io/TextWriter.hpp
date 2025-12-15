#pragma once
#include "Stream.hpp"
#include "types/Byte.hpp"
#include "types/Char.hpp"
#include "types/String.hpp"

#include <cstring>

class TextWriter {
    Stream* base;
public:
    explicit TextWriter(Stream* s) noexcept : base(s) {}
    TextWriter() noexcept : base(nullptr) {}

    bool IsValid() const noexcept { return base != nullptr && base->CanWrite(); }

    // Write raw bytes (UTF-8 sequence expected)
    void WriteBytes(const char* data, Stream::size_type len) noexcept {
        if (!IsValid() || !data || len == 0) return;
        base->Write(reinterpret_cast<const Byte*>(data), len);
    }

    // Write single Char (a single UTF-8 code unit byte)
    void WriteChar(const Char& c) noexcept {
        if (!IsValid()) return;
        unsigned char b = static_cast<unsigned char>(c);
        base->Write(reinterpret_cast<const Byte*>(&b), 1);
    }

    // Write NUL-terminated Char* (UTF-8 bytes)
    void Write(const Char* s) noexcept {
        if (!IsValid() || !s) return;

        const Char* p = s;
        size_t len = 0;
        while (*p != Char(0)) {
            ++p;
            ++len;
        }
        if (len == 0) return;

        base->Write(reinterpret_cast<const Byte*>(s),
            static_cast<Stream::size_type>(len));
    }

    // Write NUL-terminated UTF-8 char*
    void Write(const char* s) noexcept {
        if (!IsValid() || !s) return;
        size_t len = strlen(s);
        if (len == 0) return;

        base->Write(reinterpret_cast<const Byte*>(s),
            static_cast<Stream::size_type>(len));
    }

    // Write String (UTF-8)
    void Write(const String& s) noexcept {
        if (!IsValid()) return;
        const Char* data = static_cast<const Char*>(s);
        UInt64 len = s.GetByteCount(); // GetByteCount always! We need to handle CodePoints if you have it

        if (len == 0) return;
        base->Write(reinterpret_cast<const Byte*>(data),
            static_cast<Stream::size_type>(len));
    }

    void WriteLine() noexcept {
        if (!IsValid()) return;
        const char nl = '\n';
        base->Write(reinterpret_cast<const Byte*>(&nl), 1);
        base->Flush();
    }

    void Flush() noexcept {
        if (base) base->Flush();
    }

    Stream* Base() const noexcept { return base; }
};