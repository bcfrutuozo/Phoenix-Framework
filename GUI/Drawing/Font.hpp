#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"

class Font : public Object<Font>
{
public:

	Font(u32 id, const String& name) noexcept;

	u32 GetId() const noexcept;
	String GetName() const noexcept;

	Boolean IsValid() const noexcept;

private:

	u32 _id;
	String _name;
};

class FontInstance : public Object<FontInstance>
{
public:

	FontInstance() = default;
	FontInstance(Font* font, Single size, Single dpi) noexcept;

	Font* GetFont() const noexcept;
	Single GetSize() const noexcept;
	Single GetDPI() const noexcept;

	// Métricas tipográficas normalizadas
	Single GetPixelSize() const noexcept;
	Single GetLineHeight() const noexcept;
	Single GetAscent() const noexcept;
	Single GetDescent() const noexcept;

	Boolean IsValid() const noexcept;

private:

	Font* _font = nullptr;
	Single _size{ 0.0f };
	Single _dpi{ 96.0f };
};