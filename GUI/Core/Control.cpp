#include "Control.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "System/Types.hpp"
#include "Events/EventQueue.hpp"

Control::~Control()
{
    DestroyControlBackend(_impl);
}

Control::Control(const String& text, i32 x, i32 y)
    :
    _text(text),
    _x(x),
    _y(y)
{

}

void Control::SetEnabled(Boolean enabled)
{
    SetEnabledControlBackend(_impl, enabled);
}

void Control::Show()
{
    ShowControlBackend(_impl);
}

void Control::Update()
{
    UpdateControlBackend(_impl);
}

void Control::OnEvent(Event& e)
{
    EventDispatcher d(e);

    d.Dispatch<MouseButtonDownEvent>(
        EventCategory::Mouse,
        MouseEventType::ButtonDown,
        [&](const MouseButtonDownEvent& ev)
        {
            if (ev.Target.AsControl() != this)
                return;

            if(OnMouseDown) OnMouseDown(ev.Button);
            // comportamento
            e.Handled = true;
        }
    );

    d.Dispatch<MouseButtonUpEvent>(
        EventCategory::Mouse,
        MouseEventType::ButtonUp,
        [&](const MouseButtonUpEvent& ev)
        {
            if (ev.Target.AsControl() != this)
                return;

            if (OnMouseUp) OnMouseUp(ev.Button);
            // comportamento
            e.Handled = true;
        }
    );
}