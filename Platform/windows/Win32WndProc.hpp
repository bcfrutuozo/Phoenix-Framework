#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "System/Types/Boolean.hpp"

class Win32ObjectHeader;

// WndProc comum da plataforma
Boolean CALLBACK HandleWin32Message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, Win32ObjectHeader* header);
LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK Win32SubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR subclass, DWORD_PTR dwRefData);