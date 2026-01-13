#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "Events/Input/MouseEvents.hpp"

class Window;
struct WindowBackend;
struct ControlBackend;
class EventQueue;

class Control : public Object<Control>
{
public:

    struct InitializationContext
    {
        WindowBackend* WindowBackend;
    };

    virtual ~Control();

    virtual void AttachTo(Window* window) = 0;
    void SetEnabled(Boolean enabled);
    void Show();
    void Update();
    virtual void Initialize(InitializationContext ctx) = 0;

    virtual void OnEvent(Event& e);
    inline String GetText() const noexcept { return _text; }
    inline constexpr i32 GetX() const noexcept { return _x; }
    inline constexpr i32 GetY() const noexcept { return _y; }

    void (*OnFocusGained)() = nullptr;
    void (*OnMouseDown)(MouseButton) = nullptr;
    void (*OnMouseUp)(MouseButton) = nullptr;

protected:

    Control() = default;
    Control(const String& text, i32 x, i32 y);

    virtual void OnAttach() = 0;

    ControlBackend* _impl = nullptr;
    WindowBackend* _parentBackend = nullptr;

    String _text;
    i32 _x;
    i32 _y;
};