#include "Label.hpp"
#include "LabelBackend.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "GUI/Context/UIContext.hpp"
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
	_desc->UIContext = ctx.UIContext;
	if(!_desc->Font) _desc->Font = ctx.Font ? ctx.UIContext->GetDefaultFont() : ctx.Font;
	if(_desc->AutoSize) _desc->Size = CalculateControlSizeByText(this, ctx.UIContext->GetFontManager());
	_impl = CreateLabelBackend(this, _parentBackend);
}

String Label::ToString() const noexcept
{
	return _desc->Text;
}