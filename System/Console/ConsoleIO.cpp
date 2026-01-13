#include "ConsoleIO.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void ConsoleIO::SetupConsoleUTF8Once() noexcept
{
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
}

#endif