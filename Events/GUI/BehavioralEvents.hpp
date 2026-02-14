#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Core/UIHandle.hpp"

class TextInputEvent final : public Event
{
public:

	TextInputEvent(UIHandle handle, CodePoint cp)
		:
		Event(handle, Category, Type),
		Codepoint(cp)
	{
	}

	static constexpr EventCategory Category = EventCategory::Text;
	static constexpr u8 Type = static_cast<uint8_t>(TextEventType::Input);

	CodePoint Codepoint;
};

class ImeCompositionStartEvent final : public Event
{
public:

	ImeCompositionStartEvent(UIHandle handle, const String& text, UInt32 cursor)
		: 
		Event(handle, Category, Type),
		Text(text),
		Cursor(cursor)
	{
	}

	static constexpr EventCategory Category = EventCategory::Text;
	static constexpr u8 Type = static_cast<uint8_t>(TextEventType::ImeCompositionStart);

public:

	String Text;   // UTF-8 nativo da engine
	u32 Cursor; // posição do caret (em codepoints ou bytes, você decide)
};

class ImeCompositionUpdateEvent final : public Event
{
public:

	ImeCompositionUpdateEvent(UIHandle handle, const String& text, UInt32 cursor)
		:
		Event(handle, Category, Type),
		Text(text),
		Cursor(cursor)
	{
	}

	static constexpr EventCategory Category = EventCategory::Text;
	static constexpr u8 Type = static_cast<uint8_t>(TextEventType::ImeCompositionUpdate);

public:

	String Text;   // UTF-8 nativo da engine
	UInt32 Cursor; // posição do caret (em codepoints ou bytes, você decide)
};

class ImeCompositionEndEvent final : public Event
{
public:

	ImeCompositionEndEvent(UIHandle handle, const String& text, UInt32 cursor)
		:
		Event(handle, Category, Type),
		Text(text),
		Cursor(cursor)
	{
	}

	static constexpr EventCategory Category = EventCategory::Text;
	static constexpr u8 Type = static_cast<uint8_t>(TextEventType::ImeCompositionEnd);

public:

	String Text;   // UTF-8 nativo da engine
	UInt32 Cursor; // posição do caret (em codepoints ou bytes, você decide)
};

class ImeCompositionCommitEvent final : public Event
{
public:

	ImeCompositionCommitEvent(UIHandle handle, const String& text, UInt32 cursor)
		:
		Event(handle, Category, Type),
		Text(text),
		Cursor(cursor)
	{
	}

	static constexpr EventCategory Category = EventCategory::Text;
	static constexpr u8 Type = static_cast<uint8_t>(TextEventType::ImeCompositionCommit);

public:

	String Text;   // UTF-8 nativo da engine
	UInt32 Cursor; // posição do caret (em codepoints ou bytes, você decide)
};