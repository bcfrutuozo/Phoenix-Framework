#pragma once

#include "Events/Event.hpp"
#include "System/Input/KeyCode.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class ClipboardCopyEvent final : public Event
{
public:

    ClipboardCopyEvent(UIHandle handle, const String& text)
        :
        Event(handle, Category, Type),
        Text(text)
    {
    }

    static constexpr EventCategory Category = EventCategory::Clipboard;
    static constexpr u8 Type = static_cast<uint8_t>(ClipboardEventType::Copy);

    String Text;
};

class ClipboardPasteEvent final : public Event
{
public:

    ClipboardPasteEvent(UIHandle handle, const String& text)
        :
        Event(handle, Category, Type),
        Text(text)
    {
    }

    static constexpr EventCategory Category = EventCategory::Clipboard;
    static constexpr u8 Type = static_cast<uint8_t>(ClipboardEventType::Paste);

    String Text;
};

class ClipboardCutEvent final : public Event
{
public:

    ClipboardCutEvent(UIHandle handle, const String& text)
        :
        Event(handle, Category, Type),
        Text(text)
    {
    }

    static constexpr EventCategory Category = EventCategory::Clipboard;
    static constexpr u8 Type = static_cast<uint8_t>(ClipboardEventType::Cut);

    String Text;
};

class ClipboardChangedEvent final : public Event
{
public:

    ClipboardChangedEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::Clipboard;
    static constexpr u8 Type = static_cast<uint8_t>(ClipboardEventType::Changed);
};