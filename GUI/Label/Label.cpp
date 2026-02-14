#include "Label.hpp"
#include "GUI/Core/InitializationContext.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "GUI/Context/UIContext.hpp"
#include "GUI/Window/Window.hpp"

class Window;

Label::Label(const String& text, i32 x, i32 y)
	:
	Control(text, x, y, 100, 23)
{

}

void Label::InitializeImpl(InitializationContext& ctx)
{
	//_parent = ctx.Parent;
	//_uiContext = ctx.UIContext;
	//if (!_font) _font = ctx.Font ? ctx.UIContext->GetDefaultFont() : ctx.Font;
	_impl = CreateLabelBackend(this, GetParentBackend(), ctx.Queue, ctx.UIContext, ctx.EventSink);
	SetState(Flags::Created, true);
	if(GetState(Flags::AutoSize))
		RedrawWithSize(CalculateControlSizeByText(_impl));
}

String Label::ToString() const noexcept
{
	return String::Concat("Label, Text: ", _text);
}