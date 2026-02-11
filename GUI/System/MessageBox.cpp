#include "MessageBox.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "GUI/Window/Window.hpp"

MessageBox::~MessageBox()
{
	delete _desc;
	_desc = nullptr;
}

MessageBoxResult MessageBox::Show(const String& caption, const String& text)
{
	auto m = MessageBox(nullptr, caption, text, MessageBoxButtons::OK, MessageBoxIcon::None, MessageBoxDefaultButton::Button1);
	return CreateMessageBox(m._impl, m._desc);
}

MessageBoxResult MessageBox::Show(const String& caption, const String& text, MessageBoxButtons buttons)
{
	auto m = MessageBox(nullptr, caption, text, buttons, MessageBoxIcon::None, MessageBoxDefaultButton::Button1);
	return CreateMessageBox(m._impl, m._desc);
}

MessageBoxResult MessageBox::Show(const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon)
{
	auto m = MessageBox(nullptr, caption, text, buttons, icon, MessageBoxDefaultButton::Button1);
	return CreateMessageBox(m._impl, m._desc);
}

MessageBoxResult MessageBox::Show(const Window* parent, const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon)
{
	auto m = MessageBox(parent, caption, text, buttons, icon, MessageBoxDefaultButton::Button1);
	return CreateMessageBox(m._impl, m._desc);
}

MessageBox::MessageBox(const Window* parent, const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon, MessageBoxDefaultButton defaultButton)
	:
	_impl(parent != nullptr ? parent->_impl : nullptr)
{
	_desc = new MessageBoxDescriptor();
	_desc->Title = caption;
	_desc->Text = text;
	_desc->Icon = icon;
	_desc->Buttons = buttons;
	_desc->DefaultButton = defaultButton;
}