#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Font.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "HorizontalAlignment.hpp"
#include "VerticalAlignment.hpp"
#include "TextFormat.hpp"
#include "ControlDescriptor.hpp"

class Window;
struct WindowBackend;
struct ControlBackend;
class EventQueue;
class UIContext;

class Control : public Object<Control>
{
public:

    struct InitializationContext
    {
        WindowBackend* WindowBackend;
        Font* Font;
        UIContext* UIContext;
    };

    virtual ~Control();

    virtual void AttachTo(Window* window) = 0;
    void SetEnabled(Boolean enabled);
    void Show();
    void Update();
    virtual void Initialize(InitializationContext ctx) = 0;

    virtual void OnEvent(Event& e);
    inline String GetText() const noexcept { return _desc->Text; }
    inline constexpr Point GetLocation() const noexcept { return _desc->Location; }
    inline constexpr void SetLocation(Point location) noexcept { _desc->Location = location; }
    inline constexpr Size GetSize() const noexcept { return _desc->Size; }
    
    inline constexpr void SetSize(Size s) noexcept 
    {
        if (_desc->AutoSize) return;
        _desc->Size = s; 
    }

    inline constexpr Font* GetFont() const noexcept { return _desc->Font; }
    void SetFont(Font* font) noexcept;

    inline constexpr Color GetBackgroundColor() const noexcept { return _desc->BackgroundColor; }
    inline constexpr Color GetForeColor() const noexcept { return _desc->ForeColor; }

    inline constexpr void SetBackgroundColor(Color c) noexcept { _desc->BackgroundColor = c; }
    inline constexpr void SetForeColor(Color c) noexcept { _desc->ForeColor = c; }

    inline constexpr HorizontalAlignment GetHorizontalAlignment() const noexcept { return _desc->HorizontalAlignment; }
    inline constexpr VerticalAlignment GetVerticalAlignment() const noexcept { return _desc->VerticalAlignment; }
    inline constexpr TextFormat GetTextFormat() const noexcept { return _desc->TextFormat; }
    inline constexpr void SetHorizontalAlignment(HorizontalAlignment ha) { _desc->HorizontalAlignment = ha; }
    inline constexpr void SetVerticalAlignment(VerticalAlignment va) { _desc->VerticalAlignment = va; }
    inline constexpr void SetTextFormat(TextFormat tf) { _desc->TextFormat = tf; }

    inline constexpr Boolean IsAutoSizeEnabled() const noexcept { return _desc->AutoSize; }
    inline constexpr void EnableAutoSize() noexcept { _desc->AutoSize = true; }
    inline constexpr void DisableAutoSize() noexcept { _desc->AutoSize = false; }

    void (*OnFocusGained)() = nullptr;
    void (*OnDPIChanged)() = nullptr;
    void (*OnMouseDown)(MouseButtonDownEvent*) = nullptr;
    void (*OnMouseUp)(MouseButtonUpEvent*) = nullptr;

protected:

    Control() = default;
    Control(const String& text, i32 x, i32 y);

    virtual void OnAttach() = 0;

    ControlBackend* _impl = nullptr;
    WindowBackend* _parentBackend = nullptr;
    ControlDescriptor* _desc = nullptr;
};