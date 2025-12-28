#pragma once

#include "types/Object.hpp"
#include "types/String.hpp"
#include "types/Char.hpp"

class Locale : public Object<Locale>
{
public:

	Locale() : Locale("en") { }
	Locale(const char* c) : Locale(String(c)) {}
	Locale(const wchar_t* c): Locale(String(c)) {}
	Locale(const char8_t* c) : Locale(String(c)) {}
	Locale(const char16_t* c) : Locale(String(c)) {}
	Locale(const char32_t* c) : Locale(String(c)) {}
	Locale(const Char* c) : Locale(String(c)) {}
	Locale(const String& locale) : _locale(locale) {}

	inline constexpr UInt64 GetByteCount() const noexcept { return _locale.GetByteCount(); }
	
	inline Boolean IsTurkish() const noexcept 
	{
		return _locale == "tr" || _locale == "tr-TR" || _locale == "az" || _locale == "az-AZ";
	}

	inline const char* data() const noexcept { return static_cast<const char*>(_locale); }

	String ToString() const noexcept
	{
		return _locale;
	}

private:

	String _locale;
};