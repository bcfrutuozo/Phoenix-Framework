#include "OSStream.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#else
#include <unistd.h>
#include <errno.h>
#endif

OSStream::size_type OSStream::Read(Byte* buffer, size_type count) noexcept 
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE h = reinterpret_cast<HANDLE>(_get_osfhandle(FD));

    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {

        // ----------------------------
        // 1) Se ainda há bytes UTF-8 no buffer interno → consumir
        // ----------------------------
        if (utf8Pos < utf8Len) {
            size_type n = (count < (utf8Len - utf8Pos))
                ? count : (utf8Len - utf8Pos);

            memcpy(buffer, utf8Buf + utf8Pos, n);
            utf8Pos += n;
            return n;
        }

        // ----------------------------
        // 2) O buffer está vazio → ler nova linha UTF-16
        // ----------------------------
        wchar_t wbuf[256];
        DWORD wread = 0;

        if (!ReadConsoleW(h, wbuf, 255, &wread, nullptr))
            return 0;

        if (wread == 0)
            return 0;

        // ----------------------------
        // 3) Converter UTF-16 → UTF-8 para o buffer interno
        // ----------------------------
        utf8Len = WideCharToMultiByte(
            CP_UTF8, 0,
            wbuf, wread,
            utf8Buf, sizeof(utf8Buf),
            nullptr, nullptr
        );

        utf8Pos = 0;

        if (utf8Len == 0)
            return 0;

        // ----------------------------
        // 4) Entregar até count bytes ao caller
        // ----------------------------
        size_type n = (count < utf8Len) ? count : utf8Len;
        memcpy(buffer, utf8Buf, n);
        utf8Pos = n;
        return n;
    }

    // ----------------------------
    // NÃO É CONSOLE → usar _read normal
    // ----------------------------
    int r = _read(FD, buffer, static_cast<unsigned int>(count));
    if (r < 0) return 0;
    return static_cast<size_type>(r);

#else
    // Linux/macOS UTF-8 nativo
    ssize_t r = ::read(FD, buffer, count);
    if (r < 0) return 0;
    return static_cast<size_type>(r);
#endif
}

OSStream::size_type OSStream::Write(const Byte* buffer, size_type count) noexcept {
#if defined(_WIN32) || defined(_WIN64)
    int w = _write(FD, buffer, static_cast<unsigned int>(count));
    if (w < 0) return 0;
    return static_cast<size_type>(w);
#else
    ssize_t w = ::write(FD, buffer, count);
    if (w < 0) return 0;
    return static_cast<size_type>(w);
#endif
}

void OSStream::Flush() noexcept {
    // Nothing required for raw file descriptors
}