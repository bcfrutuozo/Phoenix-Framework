#pragma once

#ifdef _WIN32

#include <WinSock2.h>

#include "Globals.hpp"

class NetworkRuntime
{
public:
    static void EnsureInitialized() noexcept
    {
        if (IsWin32NetworkInitialized)
            return;

        WSADATA data;
        if (WSAStartup(MAKEWORD(2, 2), &data) == 0)
        {
            IsWin32NetworkInitialized = true;
        }
    }
};

#endif