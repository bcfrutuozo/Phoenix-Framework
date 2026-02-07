#include "UIContext.hpp"

UIContext::UIContext() noexcept
{
	_defaultFont = new Font("Segoe UI");
	_fontManager.Initialize(_defaultFont, true);
}

UIContext::~UIContext() noexcept
{
	delete _defaultFont;
}

void UIContext::InitializeFont(Font* instance)
{
	_fontManager.Initialize(instance, true);
}