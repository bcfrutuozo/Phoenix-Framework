#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Core/UIHandle.hpp"

class TextInputEvent final : public Event
{
public:

    TextInputEvent(UIHandle target, CodePoint cp)
        :
        Target(target),
        Codepoint(cp)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Text;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(TextEventType::Input);
    }

    UIHandle Target;
    CodePoint Codepoint;
};

enum class ImeCompositionType : uint8_t
{
    Start,
    Update,
    End,
    Commit
};

class ImeCompositionEvent final : public Event
{
public:
    ImeCompositionEvent(
        UIHandle target,
        ImeCompositionType type,
        const String& text,
        UInt32 cursor
    )
        : Target(target)
        , Type(type)
        , Text(text)
        , Cursor(cursor)
    {
    }

    static constexpr UInt32 StaticTypeId() noexcept
    {
        return static_cast<UInt32>(0x1001);
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Text;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(TextEventType::ImeComposition);
    }

public:
    UIHandle Target;
    ImeCompositionType Type;

    String Text;   // UTF-8 nativo da engine
    UInt32 Cursor; // posição do caret (em codepoints ou bytes, você decide)
};