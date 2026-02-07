#pragma once

#include "Win32Types.hpp"

// WndProc comum da plataforma
LRESULT __stdcall Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT __stdcall Win32LabelProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);