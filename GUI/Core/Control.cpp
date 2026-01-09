#include "Control.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "System/Events/EventDispatcher.hpp"
#include "GUI/Events/MouseEvents.hpp"
#include "GUI/Events/UIEvents.hpp"
#include "System/Types.hpp"
#include "System/Events/EventQueue.hpp"

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
            if (ev.Handle.AsControl() != this)
                return;

            if(OnMouseDown) OnMouseDown(ev.Button);
            // comportamento
            e.Handled = true;
        }
    );
}

void Control::AttachEventQueue(EventQueue* queue)
{
    AttachEventQueueToControlBackend(_impl, queue);
}

Window* Control::GetParent()
{
    return _parent;
}