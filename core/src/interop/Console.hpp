#pragma once
#include "types/Char.hpp"
#include "types/String.hpp"
#include "ConsoleIO.hpp"
#include "io/StreamOperators.hpp"

class Console {
public:
    // Write arbitrary objects via the StreamOperators to stdout
    template<typename T>
    static inline void Write(const T& v) noexcept {
        auto &io = ConsoleIO::Instance();
        StreamOperators::operator<<(io.Out, v);
    }

    template<typename T>
    static inline void WriteLine(const T& v) noexcept {
        Write(v);
        Console::WriteLine();
    }

    static inline void WriteLine() noexcept {
        auto &io = ConsoleIO::Instance();
        io.Out.WriteLine();
    }

    static inline void Flush() noexcept {
        ConsoleIO::Instance().FlushAll();
    }

    static inline String ReadLine() noexcept
    {
        auto &io = ConsoleIO::Instance().In;
        return io.ReadLine();
    }

    // Convenience wrappers for String/Char
    static inline void Write(const String& s) noexcept { Write<String>(s); }
    static inline void WriteLine(const String& s) noexcept { WriteLine<String>(s); }

    static inline TextWriter& Out() noexcept { return ConsoleIO::Instance().Out; }
    static inline TextWriter& Error() noexcept { return ConsoleIO::Instance().Error; }
    static inline TextReader& In() noexcept { return ConsoleIO::Instance().In; }
};
