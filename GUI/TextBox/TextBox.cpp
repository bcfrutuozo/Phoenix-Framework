#include "TextBox.hpp"
#include "GUI/Context/UIContext.hpp"
#include "GUI/Core/NativeBackend.hpp"

TextBox::TextBox(const String& text, i32 x, i32 y)
	:
	Control(text, x, y, 0, 0)
{

}

TextBox::~TextBox()
{

}

void TextBox::Initialize(InitializationContext ctx)
{
	_parent = ctx.Parent;
	_uiContext = ctx.UIContext;
	if (!_font) _font = ctx.Font ? ctx.UIContext->GetDefaultFont() : ctx.Font;
	_impl = CreateTextBoxBackend(this, GetParentBackend(), ctx.Queue, ctx.UIContext);
	if (_autoSize)
		RedrawWithSize(CalculateControlSizeByText(_impl));
}

String TextBox::ToString() const noexcept
{
    String stg = _text;
    if (_text.GetLength() > 40)
    {
        stg = String::Concat(_text.Substring(0, 40), "...");
    }

    return String::Concat("TextBox, Text: ", stg);
}