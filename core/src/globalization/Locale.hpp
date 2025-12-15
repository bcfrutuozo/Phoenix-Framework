#pragma once

#include "types/Object.hpp"
#include "types/String.hpp"

class Locale : public Object<Locale>
{
public:

	Locale() : Locale("en") { }
	explicit Locale(const String& locale) : _locale(locale) {  };

	inline constexpr UInt64 GetByteCount() const noexcept { return _locale.GetByteCount(); }
	
	inline constexpr Boolean IsTurkish() const noexcept 
	{
		return _locale == "tr" || _locale == "tr-TR";
	}

	inline const char* data() const noexcept { return static_cast<const char*>(_locale); }

private:

	String _locale;
};