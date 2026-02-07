#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"

class Font final: public Object<Font>
{
public:

	Font() = delete;
	Font(const String& name) noexcept;
	Font(const String& name, Single size) noexcept;
	Font(const String& name, Single size, Single dpi) noexcept;

	inline friend Boolean operator==(const Font& a, const Font& b) noexcept { return a.Equals(b); }
	inline friend Boolean operator!=(const Font& a, const Font& b) noexcept { return !(a == b); }

	String GetName() const noexcept;
	Single GetSize() const noexcept;
	Single GetDPI() const noexcept;

	// Métricas tipográficas normalizadas
	Single GetPixelSize() const noexcept;
	Single GetLineHeight() const noexcept;
	Single GetAscent() const noexcept;
	Single GetDescent() const noexcept;

	Boolean IsValid() const noexcept;
	String ToString() const noexcept;

	Boolean Equals(const Font& other) const noexcept;
	u32 GetHashCode() const noexcept;

private:

	String _name;
	Single _size;
	Single _dpi;
};