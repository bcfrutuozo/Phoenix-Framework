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
	OnAttach();
}

void Label::OnAttach()
{
	// nada por enquanto

}

void Label::Initialize(InitializationContext ctx)
{
	_parentBackend = ctx.WindowBackend;
	_impl = CreateLabelBackend(this, _parentBackend, GetText(), GetX(), GetY());
}

String Label::ToString() const noexcept
{
	return _text;
}