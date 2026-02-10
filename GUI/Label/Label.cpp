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

void Label::Initialize(InitializationContext ctx)
{
	_parentBackend = ctx.WindowBackend;
	_desc->UIContext = ctx.UIContext;
	if(!_desc->Font) _desc->Font = ctx.Font ? ctx.UIContext->GetDefaultFont() : ctx.Font;
	_impl = CreateLabelBackend(this, _parentBackend);
	if (_desc->AutoSize) 
	{
		_desc->Size = CalculateControlSizeByText(_impl);
		ResizeControl(_impl);
	}
}

String Label::ToString() const noexcept
{
	return _desc->Text;
}