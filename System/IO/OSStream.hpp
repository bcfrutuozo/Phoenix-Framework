#pragma once

#include "Stream.hpp"

class OSStream : public Stream
{
    int FD;

#if defined(_WIN32) || defined(_WIN64)
    // buffer UTF-8 interno
    char utf8Buf[512];
    UInt64 utf8Len = 0;
    UInt64 utf8Pos = 0;
#endif


public:
    explicit OSStream(int fd) noexcept : FD(fd), utf8Buf(0) {}
    virtual ~OSStream() = default;

    virtual size_type Read(Byte* buffer, size_type count) noexcept override;
    virtual size_type Write(const Byte* buffer, size_type count) noexcept override;
    virtual void Flush() noexcept override;

    virtual bool CanRead() const noexcept override { return true; }
    virtual bool CanWrite() const noexcept override { return true; }
};
