#pragma once

#include "System/Types/Boolean.hpp"
#include "Win32Types.hpp"

class Win32ObjectHeader;

// WndProc comum da plataforma
Boolean __stdcall HandleWin32Message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, Win32ObjectHeader* header);
LRESULT __stdcall Phoenix_Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT __stdcall Phoenix_Win32SubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR subclass, DWORD_PTR dwRefData);