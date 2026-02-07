#include "Control.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "System/Types.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Context/UIContext.hpp"

Control::~Control()
{
    delete _desc;
    DestroyControlBackend(_impl);
}

Control::Control(const String& text, i32 x, i32 y)
{
    _desc = new ControlDescriptor();
    _desc->AutoSize = true;
    _desc->BackgroundColor = Color::White;
    _desc->ForeColor = Color::Black;
    _desc->Location = Point(x, y);
    _desc->Size = Size(100, 23);
    _desc->Text = text;
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
        [&](MouseButtonDownEvent& ev)
        {
            if (ev.Target.AsControl() != this)
                return;

            if(OnMouseDown) OnMouseDown(&ev);
            // comportamento
            e.Handled = true;
        }
    );

    d.Dispatch<MouseButtonUpEvent>(
        EventCategory::Mouse,
        MouseEventType::ButtonUp,
        [&](MouseButtonUpEvent& ev)
        {
            if (ev.Target.AsControl() != this)
                return;

            if (OnMouseUp) OnMouseUp(&ev);
            // comportamento
            e.Handled = true;
        }
    );
}

void Control::SetFont(Font* font) noexcept
{
    _desc->Font = font;
}