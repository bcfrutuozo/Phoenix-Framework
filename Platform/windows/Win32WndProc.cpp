#include "Win32WndProc.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "GUI/Window/WindowBackend.hpp"
#include "Events/Categories/KeyboardEvents.hpp"
#include "Events/Categories/UIEvents.hpp"
#include "Events/Categories/MouseEvents.hpp"
#include "Events/Categories/TextEvents.hpp"
#include "Events/Categories/SystemEvents.hpp"
#include "Win32KeyMap.hpp"
#include "Win32PowerMap.hpp"
#include "System/Console/Console.hpp"

#ifdef _DEBUG
#include "WIn32Message.hpp"
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#define WINVER       0x0601

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

#ifndef _DEBUG
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#include <imm.h>
#pragma comment(lib, "imm32.lib")


Boolean CALLBACK HandleWin32Message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, Win32ObjectHeader* header)
{
	if (!header)
		return false;

	if (header->Kind == Win32ObjectKind::Window)
	{
		auto* backend = static_cast<WindowBackend*>(header);
		Window* window = backend->owner;

		// 🔑 HANDLE CORRETO
		UIHandle handle = UIHandle::FromWindow(window);

		switch (msg)
		{
			// -------------------- Window --------------------
		case WM_NCDESTROY:
		{
			auto* backend = reinterpret_cast<WindowBackend*>(
				GetWindowLongPtr(hwnd, GWLP_USERDATA)
				);

			if (backend)
			{
				SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
				delete backend;
			}

			return true;
		}
		case WM_SIZE:
		{
			uint32_t width = LOWORD(lp);
			uint32_t height = HIWORD(lp);

			// Resize sempre acontece
			backend->queue->Push(new UIResizeEvent(
				handle,
				width,
				height
			));

			// Estados especiais
			switch (wp)
			{
			case SIZE_MINIMIZED:
				backend->queue->Push(new UIMinimizeEvent(handle));
				break;

			case SIZE_MAXIMIZED:
				backend->queue->Push(new UIMaximizeEvent(handle));
				break;

			case SIZE_RESTORED:
				// opcional: WindowRestoreEvent, se quiser
				break;
			}

			return true;
		}
		case WM_MOVE:
			backend->queue->Push(new UIMoveEvent(handle, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp)));
			return true;

		case WM_SETFOCUS:
			backend->queue->Push(new UIFocusGainedEvent(handle));
			return true;

		case WM_KILLFOCUS:
			backend->queue->Push(new UIFocusLostEvent(handle));
			return true;

		case WM_SHOWWINDOW:
			if (wp)
				backend->queue->Push(new UIShowEvent(handle));
			else
				backend->queue->Push(new UIHideEvent(handle));
			return true;

		case WM_DPICHANGED:
			backend->queue->Push(new UIDPIChangedEvent(handle, HIWORD(wp)));
			return true;

			// -------------------- Mouse --------------------

		case WM_MOUSEMOVE:
			backend->queue->Push(new MouseMoveEvent(handle, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
			return true;

		case WM_LBUTTONDOWN:
			backend->queue->Push(new MouseButtonDownEvent(handle, MouseButton::Left));
			return true;

		case WM_LBUTTONUP:
			backend->queue->Push(new MouseButtonUpEvent(handle, MouseButton::Left));
			return true;

		case WM_RBUTTONDOWN:
			backend->queue->Push(new MouseButtonDownEvent(handle, MouseButton::Right));
			return true;

		case WM_RBUTTONUP:
			backend->queue->Push(new MouseButtonUpEvent(handle, MouseButton::Right));
			return true;

		case WM_MOUSEWHEEL:
			backend->queue->Push(new MouseScrollEvent(handle, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA));
			return true;

		case WM_MOUSEHWHEEL:
			backend->queue->Push(new MouseScrollEvent(handle, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA, 0.0f));
			return true;

			// -------------------- Keyboard --------------------

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			backend->queue->Push(new KeyDownEvent(handle, TranslateKey((uint32_t)wp, (uint32_t)lp), (lp & (1 << 30)) != 0));
			return true;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			backend->queue->Push(new KeyUpEvent(handle, TranslateKey((uint32_t)wp, (uint32_t)lp)));
			return true;

			// -------------------- Text / IME --------------------

		case WM_CHAR:
			backend->queue->Push(new TextInputEvent(handle, static_cast<uint32_t>(wp)));
			return true;

			// IME reservado (implementar depois)
		case WM_IME_STARTCOMPOSITION:
		{
			backend->queue->Push(new ImeCompositionEvent(
				handle,
				ImeCompositionType::Start,
				String::Empty(),
				0
			));
			return true;
		}
		case WM_IME_COMPOSITION:
		{
			HIMC imc = ImmGetContext(hwnd);
			if (!imc)
				break;

			// ---------------- Update (texto em composição) ----------------
			if (lp & GCS_COMPSTR)
			{
				LONG bytes = ImmGetCompositionStringW(
					imc,
					GCS_COMPSTR,
					nullptr,
					0
				);

				if (bytes > 0)
				{
					const uint32_t len = bytes / sizeof(wchar_t);

					List<wchar_t> buffer;
					buffer.Resize(len + 1);

					ImmGetCompositionStringW(
						imc,
						GCS_COMPSTR,
						buffer.Data(),
						bytes
					);

					buffer[len] = L'\0';

					String text(buffer.Data(), len);

					LONG cursor = ImmGetCompositionStringW(
						imc,
						GCS_CURSORPOS,
						nullptr,
						0
					);

					backend->queue->Push(new ImeCompositionEvent(
						handle,
						ImeCompositionType::Update,
						text,
						static_cast<UInt32>((uint32_t)cursor)
					));
				}
			}

			// ---------------- Commit (texto final) ----------------
			if (lp & GCS_RESULTSTR)
			{
				LONG bytes = ImmGetCompositionStringW(
					imc,
					GCS_RESULTSTR,
					nullptr,
					0
				);

				if (bytes > 0)
				{
					const uint32_t len = bytes / sizeof(wchar_t);

					List<wchar_t> buffer;
					buffer.Resize(len + 1);

					ImmGetCompositionStringW(
						imc,
						GCS_RESULTSTR,
						buffer.Data(),
						bytes
					);

					buffer[len] = L'\0';

					String text(buffer.Data(), len);

					backend->queue->Push(new ImeCompositionEvent(
						handle,
						ImeCompositionType::Commit,
						text,
						text.GetByteCount() // ou ByteCount, conforme sua API
					));
				}
			}

			ImmReleaseContext(hwnd, imc);
			return true;
		}
		case WM_IME_ENDCOMPOSITION:
		{
			backend->queue->Push(new ImeCompositionEvent(
				handle,
				ImeCompositionType::End,
				String::Empty(),
				0
			));
			return true;
		}
		// -------------------- System --------------------

		case WM_QUIT:
			backend->queue->Push(new SystemQuitEvent());
			return true;

		case WM_ENDSESSION:
			backend->queue->Push(new SystemShutdownEvent());
			return true;
		case WM_CLOSE:
		{
			backend->queue->Push(new UICloseEvent(handle));

			// IMPORTANTE:
			// NÃO chame DestroyWindow aqui automaticamente
			// Deixe a engine decidir
			return true;
		}
		case WM_DESTROY:
		{
			backend->queue->Push(new UIDestroyEvent(handle));
			return true;
		}
		case WM_POWERBROADCAST:
			switch (TranslatePowerEvent(wp))
			{
			case PowerEventType::BatteryLow: backend->queue->Push(new SystemBatterLowPowerEvent());
				break;
			case PowerEventType::Resume: backend->queue->Push(new SystemResumePowerEvent());
				break;
			case PowerEventType::Suspend: backend->queue->Push(new SystemSuspendPowerEvent());
				break;
			case PowerEventType::Unknown: backend->queue->Push(new SystemUnknownPowerEvent());
				break;
			}

			return true;
		}
	}
	else
	{
		auto* backend = static_cast<ControlBackend*>(header);
		Control* control = backend->owner;

		// 🔑 HANDLE CORRETO
		UIHandle handle = UIHandle::FromControl(control);

		// eventos de controle
		switch (msg)
		{
		case WM_PAINT:
			OutputDebugStringA("WM_PAINT\n");
			break;
		case WM_CREATE:
			return true;
		case WM_MOUSEMOVE:
			backend->queue->Push(new MouseMoveEvent(handle, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
			return true;

		case WM_LBUTTONDOWN:
			backend->queue->Push(
				new MouseButtonDownEvent(handle, MouseButton::Left)
			);
			return true;

		case WM_SETFOCUS:
			backend->queue->Push(new UIFocusGainedEvent(handle));
			return true;

		case WM_KILLFOCUS:
			backend->queue->Push(new UIFocusLostEvent(handle));
			return true;
		}
	}

	return false;
}

LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

#ifdef _DEBUG
	Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	// 🔑 SPECIAL CASE BECAUSE WM_NCCREATE RETURNS 1 WHEN SUCCESS INSTEAD OF 0
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* h = static_cast<Win32ObjectHeader*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)h);

		auto* backend = static_cast<WindowBackend*>(h);
		backend->hwnd = hwnd;

		return 1; // 🔥 OBRIGATÓRIO
	}

	if (HandleWin32Message(hwnd, msg, wp, lp, header))
		return 0;

	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT CALLBACK Win32SubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR dubclass, DWORD_PTR dwRefData)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(dwRefData);

#ifdef _DEBUG
	Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	if (HandleWin32Message(hwnd, msg, wp, lp, header))
		return 0; // tratado pela engine

	// 🔥 OBRIGATÓRIO
	return DefSubclassProc(hwnd, msg, wp, lp);
}