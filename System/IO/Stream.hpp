#pragma once

#include "System/Types/Byte.hpp"
#include "System/Types/UInt64.hpp"

class Stream {
public:
    using size_type = UInt64;

    virtual ~Stream() = default;

    // Read up to count bytes into buffer, return number of bytes read, 0 on EOF
    virtual size_type Read(Byte* buffer, size_type count) noexcept = 0;

    // Write count bytes from buffer. Return number of bytes written.
    virtual size_type Write(const Byte* buffer, size_type count) noexcept = 0;

    // Flush underlying stream
    virtual void Flush() noexcept = 0;

    // Capability flags
    virtual bool CanRead() const noexcept = 0;
    virtual bool CanWrite() const noexcept = 0;
};
