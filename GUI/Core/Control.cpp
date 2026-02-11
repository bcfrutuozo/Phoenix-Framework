#include "Control.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "System/Types.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Context/UIContext.hpp"

Control::Control() noexcept
{
}

Control::Control(const String& text) noexcept
	:
	Control()
{
	_text = text;
}

Control::Control(const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
	:
	Control(text)
{
	_x = left;
	_y = top;
	_width = width;
	_height = height;
}

Control::Control(Control* parent, const String& text) noexcept
	:
	Control(text)
{
	_parent = parent;
}

Control::Control(Control* parent, const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
	:
	Control(text, left, top, width, height)
{
	_parent = parent;
}

Control::~Control()
{
	if (_impl && !_backendDestroyed)
	{
		DestroyWindowBackend(_impl);
		for (auto c : _controls)
		{
			delete c;
		}
	}

	_impl = nullptr;
}

Boolean Control::IsHandledCreated() const noexcept
{
	return _impl != nullptr;
}

void Control::Close()
{
	if (!_impl || _backendDestroyed)
		return;

	// Isso dispara WM_CLOSE → WM_DESTROY
	DestroyWindowBackend(_impl);
}

Window* Control::FindWindow() noexcept
{
	Control* c = this;

	while (c->_parent != nullptr)
		c = _parent;

	return (Window*)c;
}

void Control::EnableResize()
{
	if (IsResizable()) return;

	_resizable = true;
	return SetWindowResize(_impl);
}

void Control::DisableResize()
{
	if (!IsResizable()) return;

	_resizable = false;
	return SetWindowResize(_impl);
}

void Control::AddControl(Control* control)
{
	ControlAddedEvent ev(UIHandle::FromControl(this), UIHandle::FromControl(control));
	FindWindow()->Dispatch(ev);
	if (ev.Has(EventFlags::Handled))
	{
		return;
	}

	control->_parent = this;
	_controls.Add(control);
}

void Control::RemoveControl(Control* control)
{
	if (!control) return;

	ControlRemovedEvent ev(UIHandle::FromControl(this), UIHandle::FromControl(control));
	FindWindow()->Dispatch(ev);
	if (ev.Has(EventFlags::Handled))
	{
		return;
	}

	_controls.Remove(control);
	delete control;
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

	switch (e.Category())
	{
		case EventCategory::UI:
		{
			switch (e.TypeId())
			{
				case static_cast<uint8_t>(UIEventType::Close): d.Dispatch<CloseEvent>(
					EventCategory::UI,
					UIEventType::Close,
					[&](CloseEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnClose) OnClose(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Destroy): d.Dispatch<DestroyEvent>(
					EventCategory::UI,
					UIEventType::Destroy,
					[&](DestroyEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnDestroy) OnDestroy(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Resized): d.Dispatch<ResizedEvent>(
					EventCategory::UI,
					UIEventType::Resized,
					[&](ResizedEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnResized) OnResized(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Resizing): d.Dispatch<ResizingEvent>(
					EventCategory::UI,
					UIEventType::Resizing,
					[&](ResizingEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnResizing) OnResizing(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Move): d.Dispatch<MoveEvent>(
					EventCategory::UI,
					UIEventType::Move,
					[&](MoveEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnMove) OnMove(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Restore): d.Dispatch<RestoreEvent>(
					EventCategory::UI,
					UIEventType::Restore,
					[&](RestoreEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnRestore) OnRestore(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Minimize): d.Dispatch<MinimizeEvent>(
					EventCategory::UI,
					UIEventType::Minimize,
					[&](MinimizeEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnMinimize) OnMinimize(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Maximize): d.Dispatch<MaximizeEvent>(
					EventCategory::UI,
					UIEventType::Maximize,
					[&](MaximizeEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnMaximize) OnMaximize(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::FocusGained): d.Dispatch<FocusGainedEvent>(
					EventCategory::UI,
					UIEventType::FocusGained,
					[&](FocusGainedEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnFocusGained) OnFocusGained(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::FocusLost): d.Dispatch<FocusLostEvent>(
					EventCategory::UI,
					UIEventType::FocusLost,
					[&](FocusLostEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnFocusLost) OnFocusLost(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Show): d.Dispatch<ShowEvent>(
					EventCategory::UI,
					UIEventType::Show,
					[&](ShowEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnShow) OnShow(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Hide): d.Dispatch<HideEvent>(
					EventCategory::UI,
					UIEventType::Hide,
					[&](HideEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnHide) OnHide(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::DPIChanged): d.Dispatch<DPIChangedEvent>(
					EventCategory::UI,
					UIEventType::DPIChanged,
					[&](DPIChangedEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnDPIChanged) OnDPIChanged(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::FontChanged): d.Dispatch<FontChangedEvent>(
					EventCategory::UI,
					UIEventType::FontChanged,
					[&](FontChangedEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnFontChanged) OnFontChanged(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::Paint): d.Dispatch<PaintEvent>(
					EventCategory::UI,
					UIEventType::Paint,
					[&](PaintEvent& ev)
					{
						if (ev.Handle.AsWindow() == this)
							if (OnPaint) OnPaint(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::ControlAdded): d.Dispatch<ControlAddedEvent>(
					EventCategory::UI,
					UIEventType::ControlAdded,
					[&](ControlAddedEvent& ev)
					{
						if (ev.Target.AsWindow() == this)
							if (OnControlAdded) OnControlAdded(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(UIEventType::ControlRemoved): d.Dispatch<ControlRemovedEvent>(
					EventCategory::UI,
					UIEventType::ControlRemoved,
					[&](ControlRemovedEvent& ev)
					{
						if (ev.Target.AsWindow() == this)
							if (OnControlRemoved) OnControlRemoved(&ev);
					}
				);
					break;
			}
		}
		case EventCategory::Mouse:
		{
			switch (e.TypeId())
			{
				case static_cast<uint8_t>(MouseEventType::Move): d.Dispatch<MouseMoveEvent>(
					EventCategory::Mouse,
					MouseEventType::Move,
					[&](MouseMoveEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseMove) OnMouseMove(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(MouseEventType::ButtonDown): d.Dispatch<MouseButtonDownEvent>(
					EventCategory::Mouse,
					MouseEventType::ButtonDown,
					[&](MouseButtonDownEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseDown) OnMouseDown(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(MouseEventType::ButtonUp): d.Dispatch<MouseButtonUpEvent>(
					EventCategory::Mouse,
					MouseEventType::ButtonUp,
					[&](MouseButtonUpEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseUp) OnMouseUp(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(MouseEventType::Scroll): d.Dispatch<MouseScrollEvent>(
					EventCategory::Mouse,
					MouseEventType::Scroll,
					[&](MouseScrollEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseScroll) OnMouseScroll(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(MouseEventType::Enter): d.Dispatch<MouseEnterEvent>(
					EventCategory::Mouse,
					MouseEventType::Enter,
					[&](MouseEnterEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseEnter) OnMouseEnter(&ev);
					}
				);
					break;
				case static_cast<uint8_t>(MouseEventType::Leave): d.Dispatch<MouseLeaveEvent>(
					EventCategory::Mouse,
					MouseEventType::Leave,
					[&](MouseLeaveEvent& ev)
					{
						if (ev.Target.AsControl() == this)
							if (OnMouseLeave) OnMouseLeave(&ev);
					}
				);
					break;
			}
		}

		case EventCategory::Touch: {}
		case EventCategory::Gamepad: {}
		case EventCategory::Text: {}
		case EventCategory::Clipboard: {}
		case EventCategory::System: {}
	}

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
	if (_autoSize) return;

	_width = s.GetWidth();
	_height = s.GetHeight();

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
		FontChangedEvent ev = FontChangedEvent(UIHandle::FromControl(this), font, _font);
		OnEvent(ev);

		// User intercepted and cancelled font chang event
		if (ev.Has(EventFlags::Handled))
		{
			return;
		}

		if (_font == font) return;
		_font = font;

		if (_autoSize)
		{
			SetSize(CalculateControlSizeByText(_impl));
			ResizeControl(_impl);
		}

		UpdateControlBackend(_impl);
	}
	else
	{
		if (_font == font) return;
		_font = font;
	}
}

void Control::RedrawWithSize(Size s) noexcept
{
	_width = s.GetWidth();
	_height = s.GetHeight();

	if (IsHandledCreated())
	{
		ResizeControl(_impl);
	}
}