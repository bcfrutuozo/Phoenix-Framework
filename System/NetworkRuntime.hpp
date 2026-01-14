#pragma once

#ifdef _WIN32

#include <WinSock2.h>

#include "System/Globals.hpp"

class NetworkRuntime
{
public:
    static void EnsureInitialized() noexcept
    {
        if (Phoenix::IsWin32NetworkInitialized)
            return;

        WSADATA data;
        if (WSAStartup(MAKEWORD(2, 2), &data) == 0)
        {
            Phoenix::IsWin32NetworkInitialized = true;
        }
    }

    static void CloseNetwork() noexcept
    {
        WSACleanup();
    }
};

#endif