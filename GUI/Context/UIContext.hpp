#pragma once

#include "FontManager.hpp"

class UIContext : public Object<UIContext>
{
public:

	UIContext() noexcept;
	~UIContext() noexcept;

	void InitializeFont(Font* instance);

	inline constexpr FontManager* GetFontManager() noexcept { return &_fontManager; }
	inline constexpr Font* GetDefaultFont() const noexcept { return _defaultFont; }

private:

	FontManager _fontManager;
	Font* _defaultFont;
};