#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Events/MouseEvents.hpp"

class Window;
struct ControlBackend;
class EventQueue;

class Control : public Object<Control>
{
public:

    virtual ~Control();

    virtual void AttachTo(Window* window) = 0;
    void AttachEventQueue(EventQueue* queue);
    void SetEnabled(Boolean enabled);
    void Show();
    void Update();

    virtual void OnEvent(Event& e);
    inline constexpr String& GetText() { return _text; }
    inline constexpr i32 GetX() { return _x; }
    inline constexpr i32 GetY() { return _y; }
    Window* GetParent();


    void (*OnFocusGained)() = nullptr;
    void (*OnMouseDown)(MouseButton) = nullptr;

protected:

    Control() = default;
    Control(const String& text, i32 x, i32 y);

    virtual void OnAttach() = 0;

    ControlBackend* _impl = nullptr;
    Window* _parent = nullptr;

    String _text;
    i32 _x;
    i32 _y;
};