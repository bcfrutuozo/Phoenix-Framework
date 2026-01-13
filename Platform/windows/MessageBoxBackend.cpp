#include "GUI/System/MessageBoxBackend.hpp"
#include "GUI/System/MessageBoxDescriptor.hpp"
#include "System/String.hpp"
#include "Platform/windows/Win32ObjectHeader.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static UINT BuildMessageBoxFlags(MessageBoxButtons buttons, MessageBoxIcon icon, MessageBoxDefaultButton def)
{
	UINT flags = 0;

	// Buttons
	switch (buttons)
	{
	case MessageBoxButtons::OK:                flags |= MB_OK; break;
	case MessageBoxButtons::OKCancel:          flags |= MB_OKCANCEL; break;
	case MessageBoxButtons::YesNo:             flags |= MB_YESNO; break;
	case MessageBoxButtons::YesNoCancel:       flags |= MB_YESNOCANCEL; break;
	case MessageBoxButtons::RetryCancel:       flags |= MB_RETRYCANCEL; break;
	case MessageBoxButtons::AbortRetryIgnore:  flags |= MB_ABORTRETRYIGNORE; break;
	}

	// Icon
	switch (icon)
	{
	case MessageBoxIcon::Info:     flags |= MB_ICONINFORMATION; break;
	case MessageBoxIcon::Warning:  flags |= MB_ICONWARNING; break;
	case MessageBoxIcon::Error:    flags |= MB_ICONERROR; break;
	case MessageBoxIcon::Question: flags |= MB_ICONQUESTION; break;
	default: break;
	}

	// Default button
	switch (def)
	{
	case MessageBoxDefaultButton::Button2: flags |= MB_DEFBUTTON2; break;
	case MessageBoxDefaultButton::Button3: flags |= MB_DEFBUTTON3; break;
	default: flags |= MB_DEFBUTTON1; break;
	}

	return flags;
}

MessageBoxResult CreateMessageBox(WindowBackend* backend, const MessageBoxDescriptor* desc)
{
	return TranslateResult(
		MessageBox(
			backend != nullptr ? backend->hwnd : nullptr,
			desc->Text.ToWideCharArray().GetData(),
			desc->Title.ToWideCharArray().GetData(),
			BuildMessageBoxFlags(desc->Buttons, desc->Icon, desc->DefaultButton)
		));
}

MessageBoxResult TranslateResult(i32 win32Result)
{
	switch ((int32_t)win32Result)
	{
	case IDOK:       return MessageBoxResult::OK;
	case IDCANCEL:   return MessageBoxResult::Cancel;
	case IDYES:      return MessageBoxResult::Yes;
	case IDNO:       return MessageBoxResult::No;
	case IDRETRY:    return MessageBoxResult::Retry;
	case IDABORT:    return MessageBoxResult::Abort;
	case IDIGNORE:   return MessageBoxResult::Ignore;
	case IDCLOSE:    return MessageBoxResult::Close;
	default:         return MessageBoxResult::Timeout; // 0 ou qualquer valor inesperado
	}
}