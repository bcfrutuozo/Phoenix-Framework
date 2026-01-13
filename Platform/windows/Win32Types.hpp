#pragma once

// Tipos opacos
struct HWND__;
struct HINSTANCE__;

using HWND = HWND__*;
using HINSTANCE = HINSTANCE__*;

// Tipos básicos
using WPARAM = unsigned long long;
using LPARAM = long long;
using LRESULT = long long;
using UINT = unsigned int;
using DWORD_PTR = unsigned long long;
using UINT_PTR = unsigned long long;