#pragma once

#include "io/OSStream.hpp"
#include "io/TextWriter.hpp"
#include "io/TextReader.hpp"
#include "types/Char.hpp"
#include "types/String.hpp"

class ConsoleIO {
    OSStream stdin_stream;
    OSStream stdout_stream;
    OSStream stderr_stream;

private:

    static void SetupConsoleUTF8Once() noexcept {
#if defined(_WIN32) || defined(_WIN64)
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
            // Habilita UTF-8 no console
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);

            // Habilita suporte a UTF-8 no modo virtual terminal (para emojis e cores)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD mode = 0;
                if (GetConsoleMode(hOut, &mode)) {
                    mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(hOut, mode);
                }
            }
        }
#endif
    }

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
