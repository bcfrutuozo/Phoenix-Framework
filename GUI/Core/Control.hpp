#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Font.hpp"
#include "Events/Event.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "HorizontalAlignment.hpp"
#include "VerticalAlignment.hpp"
#include "TextFormat.hpp"
#include "ControlDescriptor.hpp"

class Window;
struct WindowBackend;
struct ControlBackend;
class EventQueue;
class UIContext;

class ControlAddedEvent;
class ControlRemovedEvent;
class FocusGainedEvent;
class DPIChangedEvent;
class FontChangedEvent;

// Behavioral Events
class TextInputEvent;
class ImeCompositionEvent;

// Keyboard Events
class KeyDownEvent;
class KeyUpEvent;

// Mouse Events
class MouseMoveEvent;
class MouseButtonDownEvent;
class MouseButtonUpEvent;
class MouseScrollEvent;

// Events
class SystemQuitEvent;
class SystemShutdownEvent;
class SystemResumePowerEvent;
class SystemBatteryLowPowerEvent;
class SystemUnknownPowerEvent;
class SystemSuspendPowerEvent;

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

    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;

    void AttachTo(Window* window) noexcept;
    void SetEnabled(Boolean enabled);
    void Show();
    void Update();
    virtual void Initialize(InitializationContext ctx) = 0;

    virtual Boolean OnEvent(Event& e);
    inline String GetText() const noexcept { return _desc->Text; }
    inline constexpr Point GetLocation() const noexcept { return _desc->Location; }
    inline constexpr i32 GetX() const noexcept { return _desc->Location.GetX(); }
    void SetX(i32 x) noexcept;
    inline constexpr i32 GetY() const noexcept { return _desc->Location.GetY(); }
    void SetY(i32 y) noexcept;
    inline constexpr void SetLocation(Point location) noexcept { _desc->Location = location; }
    inline constexpr Size GetSize() const noexcept { return _desc->Size; }
    inline constexpr i32 GetWidth() const noexcept { return _desc->Size.GetWidth(); }
    void SetWidth(i32 width) noexcept;
    inline constexpr i32 GetHeight() const noexcept { return _desc->Size.GetHeight(); }
    void SetHeight(i32 height) noexcept;
    void SetSize(i32 x, i32 y) noexcept;
    void SetSize(Size s) noexcept;

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
    Boolean IsHandledCreated() const noexcept;

    void (*OnControlAdded)(ControlAddedEvent*) = nullptr;
    void (*OnControlRemoved)(ControlRemovedEvent*) = nullptr;
    void (*OnFocusGained)(FocusGainedEvent*) = nullptr;
    void (*OnDPIChanged)(DPIChangedEvent*) = nullptr;
    void (*OnFontChanged)(FontChangedEvent*) = nullptr;

    // Behavioral Events
    void (*OnTextInputEvent)(TextInputEvent*) = nullptr;
    void (*OnImeCompositionEvent)(ImeCompositionEvent*) = nullptr;

    // Keyboard Events
    void (*OnKeyDownEvent)(KeyDownEvent*) = nullptr;
    void (*OnKeyUpEvent)(KeyUpEvent*) = nullptr;

    // Mouse Events
    void (*OnMouseMove)(MouseMoveEvent*) = nullptr;
    void (*OnMouseDown)(MouseButtonDownEvent*) = nullptr;
    void (*OnMouseUp)(MouseButtonUpEvent*) = nullptr;
    void (*OnMouseScroll)(MouseScrollEvent*) = nullptr;

    // System Events
    void (*OnSystemQuitEvent)(SystemQuitEvent*) = nullptr;
    void (*OnSystemShutdownEvent)(SystemShutdownEvent*) = nullptr;
    void (*OnSystemResumePowerEvent)(SystemResumePowerEvent*) = nullptr;
    void (*OnSystemBatteryLowPowerEvent)(SystemBatteryLowPowerEvent*) = nullptr;
    void (*OnSystemUnknownPowerEvent)(SystemUnknownPowerEvent*) = nullptr;
    void (*OnSystemSuspendPowerEvent)(SystemSuspendPowerEvent*) = nullptr;

protected:

    Control() = default;
    Control(const String& text, i32 x, i32 y);

    ControlBackend* _impl = nullptr;
    WindowBackend* _parentBackend = nullptr;
    ControlDescriptor* _desc = nullptr;
};