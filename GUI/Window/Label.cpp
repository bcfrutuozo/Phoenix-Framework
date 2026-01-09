#include "Label.hpp"
#include "LabelBackend.hpp"
#include "GUI/Window/Window.hpp"

class Window;

Label::Label(const String& text, i32 x, i32 y)
	:
	Control(text, x, y)
{

}

void Label::AttachTo(Window* window)
{
	_parent = window;
	_impl = CreateLabelBackend(this, window->GetNativeHandle(), GetText(), GetX(), GetY());
	OnAttach();
}

void Label::OnAttach()
{
	// nada por enquanto

}

String Label::ToString() const noexcept
{
	return _text;
}