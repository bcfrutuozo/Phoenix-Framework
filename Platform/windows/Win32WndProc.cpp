#include "Win32WndProc.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "Events/Input/KeyboardEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/BehavioralEvents.hpp"
#include "Events/OS/SystemEvents.hpp"
#include "Win32KeyMap.hpp"
#include "Win32PowerMap.hpp"
#include "System/Console/Console.hpp"
#include "GUI/Controls.hpp"
#include "GUI/Drawing/Font.hpp"
#include "GUI/Context/FontManager.hpp"
#include "Events\IEventSink.hpp"
#include "System/Exceptions.hpp"
#include "Win32Helpers.hpp"

#ifdef _DEBUG
#include "Win32Message.hpp"
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#define WINVER       0x0601

#include <windows.h>

#ifndef _DEBUG
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#include <imm.h>
#pragma comment(lib, "imm32.lib")

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

#ifdef _DEBUG
	//Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	// 🔑 SPECIAL CASE BECAUSE WM_NCCREATE RETURNS 1 WHEN SUCCESS INSTEAD OF 0
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* h = static_cast<Win32ObjectHeader*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)h);

		auto* backend = static_cast<NativeBackend*>(h);
		backend->Handle = hwnd;
		return true;
	}

	if (!header)
		return false;

	auto* backend = static_cast<NativeBackend*>(header);
	Window* window = (Window*)backend->Owner;

	// 🔑 HANDLE CORRETO
	UIHandle owner = UIHandle::FromWindow(window);

	switch (msg)
	{
	case WM_NCACTIVATE:
		return 1;
	case WM_CREATE:
		return 0;

		// -------------------- Window --------------------
	case WM_NCDESTROY:
	{
		auto* backend = reinterpret_cast<NativeBackend*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (backend)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
			delete backend;
		}

		return 0;
	}
	case WM_SIZE:
	{
		uint32_t width = LOWORD(lp);
		uint32_t height = HIWORD(lp);

		// Resize sempre acontece
		backend->EventQueue->Push(new ResizedEvent(owner, width, height));

		// Estados especiais
		switch (wp)
		{
		case SIZE_MINIMIZED:
			backend->EventQueue->Push(new MinimizeEvent(owner));
			break;

		case SIZE_MAXIMIZED:
			backend->EventQueue->Push(new MaximizeEvent(owner));
			break;

		case SIZE_RESTORED:
			backend->EventQueue->Push(new RestoreEvent(owner));
			break;
		}

		return 0;
	}
	case WM_SIZING:
	{
		RECT* r = (RECT*)lp;

		ResizingEvent ev = ResizingEvent(owner, (int32_t)r->right, (int32_t)r->bottom);

		// Dispatch imediato
		backend->EventSink->Dispatch(ev);
		if (ev.Has(EventFlags::Handled))
		{
			// bloqueia o resize
			Size z = backend->Owner->GetSize();
			r->right = (int32_t)z.GetWidth();
			r->bottom = (int32_t)z.GetHeight();
			return 0;
		}

		return 0;
	}
	case WM_MOVE:
		backend->EventQueue->Push(new MoveEvent(owner, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp)));
		break;

	case WM_SETFOCUS:
		backend->EventQueue->Push(new FocusGainedEvent(owner));
		break;

	case WM_KILLFOCUS:
		backend->EventQueue->Push(new FocusLostEvent(owner));
		break;

	case WM_SHOWWINDOW:
	{
		if (wp)
			backend->EventQueue->Push(new ShowEvent(owner));
		else
			backend->EventQueue->Push(new HideEvent(owner));
		return 0;
	}
	case WM_DPICHANGED:
		backend->EventQueue->Push(new DPIChangedEvent(owner, HIWORD(wp)));
		break;

		// -------------------- Mouse --------------------

	case WM_MOUSEMOVE:
	{
		if (!backend->TrackingMouse)
		{
			TRACKMOUSEEVENT tme{};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;
			tme.dwHoverTime = 100;

			TrackMouseEvent(&tme);
			backend->TrackingMouse = true;

			backend->EventQueue->Push(new MouseEnterEvent(owner, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
			return 0;
		}

		backend->EventQueue->Push(new MouseMoveEvent(owner, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
		return 0;
	}
	case WM_MOUSELEAVE:
	{
		backend->TrackingMouse = false;
		backend->EventQueue->Push(new MouseLeaveEvent(owner));
		return 0;
	}

	case WM_LBUTTONDOWN:
		backend->EventQueue->Push(new MouseButtonDownEvent(owner, MouseButton::Left));
		return 0;

	case WM_LBUTTONUP:
		backend->EventQueue->Push(new MouseButtonUpEvent(owner, MouseButton::Left));
		return 0;

	case WM_RBUTTONDOWN:
		backend->EventQueue->Push(new MouseButtonDownEvent(owner, MouseButton::Right));
		return 0;

	case WM_RBUTTONUP:
		backend->EventQueue->Push(new MouseButtonUpEvent(owner, MouseButton::Right));
		return 0;

	case WM_MOUSEWHEEL:
		backend->EventQueue->Push(new MouseScrollEvent(owner, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA));
		return 0;

	case WM_MOUSEHWHEEL:
		backend->EventQueue->Push(new MouseScrollEvent(owner, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA, 0.0f));
		return 0;

		// -------------------- Keyboard --------------------

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		backend->EventQueue->Push(new KeyDownEvent(owner, TranslateKey((uint32_t)wp, (uint32_t)lp), (lp & (1 << 30)) != 0));
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		backend->EventQueue->Push(new KeyUpEvent(owner, TranslateKey((uint32_t)wp, (uint32_t)lp)));
		return 0;

		// -------------------- Text / IME --------------------

	case WM_CHAR:
		backend->EventQueue->Push(new TextInputEvent(owner, static_cast<uint32_t>(wp)));
		return 0;

	case WM_IME_STARTCOMPOSITION:
	{
		backend->EventQueue->Push(new ImeCompositionEvent(
			owner,
			ImeCompositionType::Start,
			String::Empty(),
			0
		));
		return 0;
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

				backend->EventQueue->Push(new ImeCompositionEvent(
					owner,
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

				backend->EventQueue->Push(new ImeCompositionEvent(
					owner,
					ImeCompositionType::Commit,
					text,
					text.GetByteCount() // ou ByteCount, conforme sua API
				));
			}
		}

		ImmReleaseContext(hwnd, imc);
		return 0;
	}
	case WM_IME_ENDCOMPOSITION:
	{
		backend->EventQueue->Push(new ImeCompositionEvent(
			owner,
			ImeCompositionType::End,
			String::Empty(),
			0
		));
		return 0;
	}
	// -------------------- System --------------------

	case WM_QUIT:
		backend->EventQueue->Push(new SystemQuitEvent());
		break;

	case WM_ENDSESSION:
		backend->EventQueue->Push(new SystemShutdownEvent());
		return 0;
	case WM_CLOSE:
	{
		backend->EventQueue->Push(new CloseEvent(owner));

		// IMPORTANTE:
		// NÃO chame DestroyWindow aqui automaticamente
		// Deixe a engine decidir
		return 0;
	}
	case WM_DESTROY:
	{
		backend->EventQueue->Push(new DestroyEvent(owner));
		return 0;
	}
	case WM_POWERBROADCAST:
	{
		switch (TranslatePowerEvent(wp))
		{
		case PowerEventType::BatteryLow: backend->EventQueue->Push(new SystemBatteryLowPowerEvent());
			break;
		case PowerEventType::Resume: backend->EventQueue->Push(new SystemResumePowerEvent());
			break;
		case PowerEventType::Suspend: backend->EventQueue->Push(new SystemSuspendPowerEvent());
			break;
		case PowerEventType::Unknown: backend->EventQueue->Push(new SystemUnknownPowerEvent());
			break;
		}

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);

		PaintEvent ev(owner);

		backend->EventSink->Dispatch(ev);
		if (ev.Has(EventFlags::Handled))
		{
			EndPaint(hwnd, &ps);
			return 0;
		}

		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wp;

		RECT rc;
		GetClientRect(hwnd, &rc);

		HBRUSH brush = CreateSolidBrush(ToCOLORREF(window->GetBackgroundColor()));

		FillRect(hdc, &rc, brush);
		DeleteObject(brush);

		return 1;
	}
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT CALLBACK Win32LabelProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

#ifdef _DEBUG
	//Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	// 🔑 SPECIAL CASE BECAUSE WM_NCCREATE RETURNS 1 WHEN SUCCESS INSTEAD OF 0
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* h = static_cast<NativeBackend*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)h);

		auto* backend = static_cast<NativeBackend*>(h);
		backend->Handle = hwnd;
		return true;
	}

	if (!header)
		return false;

	auto* backend = static_cast<NativeBackend*>(header);
	Control* c = backend->Owner;

	// 🔑 HANDLE CORRETO
	UIHandle owner = UIHandle::FromControl(c);

	switch (msg)
	{
		// -------------------- Mouse --------------------
	case WM_MOUSEMOVE:
	{
		if (!backend->TrackingMouse)
		{
			TRACKMOUSEEVENT tme{};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;
			tme.dwHoverTime = 100;

			TrackMouseEvent(&tme);
			backend->TrackingMouse = true;

			backend->EventQueue->Push(new MouseEnterEvent(owner, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
			return 0;
		}

		backend->EventQueue->Push(new MouseMoveEvent(owner, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
		return 0;
	}
	case WM_MOUSELEAVE:
	{
		backend->TrackingMouse = false;
		backend->EventQueue->Push(new MouseLeaveEvent(owner));
		return 0;
	}
	case WM_LBUTTONDOWN:
		backend->EventQueue->Push(new MouseButtonDownEvent(owner, MouseButton::Left));
		return 0;

	case WM_LBUTTONUP:
		backend->EventQueue->Push(new MouseButtonUpEvent(owner, MouseButton::Left));
		return 0;

	case WM_RBUTTONDOWN:
		backend->EventQueue->Push(new MouseButtonDownEvent(owner, MouseButton::Right));
		return 0;

	case WM_RBUTTONUP:
		backend->EventQueue->Push(new MouseButtonUpEvent(owner, MouseButton::Right));
		return 0;

	case WM_MOUSEWHEEL:
		backend->EventQueue->Push(new MouseScrollEvent(owner, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA));
		return 0;

	case WM_MOUSEHWHEEL:
		backend->EventQueue->Push(new MouseScrollEvent(owner, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA, 0.0f));

		return 0;
	case WM_PAINT:
	{
		FontManager* fm = backend->Context->GetFontManager();

		PaintEvent ev = PaintEvent(owner);

		// Dispatch imediato
		backend->EventSink->Dispatch(ev);
		if (ev.Has(EventFlags::Handled))
		{
			return 0;
		}

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT rc{ 0, 0, c->GetWidth(), c->GetHeight() };

		HFONT oldFont = (HFONT)SelectObject(hdc, fm->GetNativeObject(c->GetFont()).Get());

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, ToCOLORREF(c->GetForeColor()));

		Array<wchar_t> buffer = c->GetText().ToWideCharArray();

		DrawTextExW(
			hdc,
			buffer.GetData(),
			buffer.GetLength(),
			&rc,
			Win32ConvertTextFlags(
				c->GetHorizontalAlignment(),
				c->GetVerticalAlignment(),
				c->GetTextFormat()
			),
			nullptr
		);

		SelectObject(hdc, oldFont);
		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wp;
		RECT rc{ 0,0, c->GetWidth(), c->GetHeight() };

		HBRUSH brush = CreateSolidBrush(ToCOLORREF(c->GetBackgroundColor()));
		FillRect(hdc, &rc, brush);
		DeleteObject(brush);

		return 1; // fundo já foi pintado
	}
	case WM_DPICHANGED:
		if (backend) backend->EventQueue->Push(new DPIChangedEvent(owner, HIWORD(wp)));
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT CALLBACK Win32TextBoxProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR refData)
{
	NativeBackend* backend = reinterpret_cast<NativeBackend*>(refData);
	TextBox* owner = (TextBox*)backend->Owner;

	switch (msg)
	{
		// ------------------------------------
			// TECLADO
			// ------------------------------------
	case WM_CHAR:
	{
		//if (!self->IsCharAllowed(wParam))
		return 0; // bloqueia caractere

		break; // deixa o EDIT processar
	}

	case WM_KEYDOWN:
	{
		if (wp == VK_RETURN)
		{
			// evita beep
			return 0;
		}

		return 0;
	}

	// ------------------------------------
	// CLIPBOARD
	// ------------------------------------
	case WM_PASTE:
	{
		//if (self->numericOnly)
		//	return 0; // bloqueia paste
		break;
	}

	// ------------------------------------
	// TEXTO
	// ------------------------------------
	case WM_SETTEXT:
	{
		//self->internalChange = true;
		LRESULT r = DefSubclassProc(hwnd, msg, wp, lp);
		//self->internalChange = false;
		return r;
	}

	case WM_COMMAND:
	{
		if (HIWORD(wp) == EN_CHANGE)
			//self->OnTextChanged();
			break;
	}

	// ------------------------------------
	// FOCO
	// ------------------------------------
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	{
		InvalidateRect(hwnd, nullptr, TRUE);
		break;
	}

	// ------------------------------------
	// VISUAL
	// ------------------------------------
	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wp;
		RECT rc;
		GetClientRect(backend->Handle, &rc);
		HBRUSH brush = CreateSolidBrush(ToCOLORREF(owner->GetBackgroundColor()));
		FillRect(hdc, &rc, brush);
		DeleteObject(brush);

		return TRUE; // evita flicker
	}

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wp;
		SetTextColor(hdc, ToCOLORREF(owner->GetForeColor()));
		//return (LRESULT)owner->bgBrush;
		return 0;
	}

	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(nullptr, IDC_IBEAM));
		return TRUE;
	}
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}