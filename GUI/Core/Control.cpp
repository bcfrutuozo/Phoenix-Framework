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

Boolean Control::IsHandledCreated() const noexcept
{
	return _impl != nullptr;
}

void Control::AttachTo(Window* w) noexcept
{
	_desc->Parent = w;
	//if (OnAttach) OnAttach();
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

Boolean Control::OnEvent(Event& e)
{
	EventDispatcher d(e);

	d.Dispatch<MouseButtonDownEvent>(
		EventCategory::Mouse,
		MouseEventType::ButtonDown,
		[&](MouseButtonDownEvent& ev)
		{
			if (ev.Target.AsControl() != this)
				return false;

			if (OnMouseDown) OnMouseDown(&ev);
		}
	);

	if (e.Has(EventFlags::Consumed)) return true;

	d.Dispatch<MouseButtonUpEvent>(
		EventCategory::Mouse,
		MouseEventType::ButtonUp,
		[&](MouseButtonUpEvent& ev)
		{
			if (ev.Target.AsControl() != this)
				return false;

			if (OnMouseUp) OnMouseUp(&ev);
		}
	);

	if (e.Has(EventFlags::Consumed)) return true;

	d.Dispatch<FontChangedEvent>(
		EventCategory::UI,
		UIEventType::FontChanged,
		[&](FontChangedEvent& ev)
		{
			if (ev.Target.AsControl() != this)
				return false;

			if (OnFontChanged) OnFontChanged(&ev);
		}
	);

	if (e.Has(EventFlags::Consumed)) return true;

	return false;
}

void Control::SetX(i32 x) noexcept 
{ 
	SetLocation({ x, GetY() }); 
}

void Control::SetY(i32 y) noexcept 
{
	SetLocation({ GetX(), y });
}

void Control::SetSize(i32 x, i32 y) noexcept
{
	SetSize({ x, y });
}

void Control::SetSize(Size s) noexcept
{
	if (_desc->AutoSize) return;
	_desc->Size = s;

	if (IsHandledCreated())
	{
		ResizeControl(_impl);
	}
}

void Control::SetWidth(i32 width) noexcept
{
	SetSize({ width, GetHeight() });
}

void Control::SetHeight(i32 height) noexcept
{
	SetSize({ GetWidth(), height });
}

void Control::SetFont(Font* font) noexcept
{
	if (IsHandledCreated())
	{
		FontChangedEvent ev = FontChangedEvent(UIHandle::FromControl(this), font, _desc->Font);
		OnEvent(ev);

		// User intercepted and cancelled font chang event
		if (ev.Has(EventFlags::Handled))
		{
			return;
		}

		if (_desc->Font == font) return;
		_desc->Font = font;

		if (_desc->AutoSize)
		{
			_desc->Size = CalculateControlSizeByText(_impl);
			ResizeControl(_impl);
		}

		UpdateControlBackend(_impl);
	}
	else
	{
		if (_desc->Font == font) return;
		_desc->Font = font;
	}
}