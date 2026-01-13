#pragma once

#include "System/IO/OSStream.hpp"
#include "System/IO/TextWriter.hpp"
#include "System/IO/TextReader.hpp"
#include "System/Types.hpp"

class ConsoleIO {
    OSStream stdin_stream;
    OSStream stdout_stream;
    OSStream stderr_stream;

private:

    static void SetupConsoleUTF8Once() noexcept;

public:
    TextReader In;
    TextWriter Out;
    TextWriter Error;

    ConsoleIO()
    : stdin_stream(0), stdout_stream(1), stderr_stream(2),
      In(&stdin_stream), Out(&stdout_stream), Error(&stderr_stream)
    {
        SetupConsoleUTF8Once();
    }

    static ConsoleIO& Instance() {
        static ConsoleIO inst;
        return inst;
    }

    void FlushAll() noexcept {
        Out.Flush();
        Error.Flush();
    }
};
