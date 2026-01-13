#include "Exceptions.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

SystemException::SystemException(i32 HRESULT) noexcept
{
	LPSTR buffer = nullptr;

	DWORD size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		static_cast<uint32_t>(HRESULT),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&buffer,
		0,
		nullptr
	);

	String message;

	if (size && buffer)
	{
		_message = String(buffer);
		_message.TrimEnd();
		LocalFree(buffer);
	}
	else
	{
		_message = String("Unknown HRESULT");
	}
}

#endif