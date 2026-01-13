#pragma once

#include "System/Types.hpp"
#include "MessageBoxDescriptor.hpp"

struct WindowBackend;
class Window;

class MessageBox : public Object<MessageBox>
{
public:

	~MessageBox();

	static MessageBoxResult Show(const String& caption, const String& text);
	static MessageBoxResult Show(const String& caption, const String& text, MessageBoxButtons buttons);
	static MessageBoxResult Show(const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon);
	static MessageBoxResult Show(const Window* parent, const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon);

private:

	MessageBox(const Window* parent, const String& caption, const String& text, MessageBoxButtons buttons, MessageBoxIcon icon, MessageBoxDefaultButton defaultButton);
	MessageBoxDescriptor* _desc = nullptr;
	WindowBackend* _impl = nullptr;
};