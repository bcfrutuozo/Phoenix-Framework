#include "Control.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "System/Types.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Context/UIContext.hpp"

ControlBase::ControlBase() noexcept
{
}

ControlBase::ControlBase(const String& text) noexcept
	:
	ControlBase()
{
	_text = text;
}

ControlBase::ControlBase(const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
	:
	ControlBase(text)
{
	_x = left;
	_y = top;
	_width = width;
	_height = height;
}

ControlBase::ControlBase(ControlBase* parent, const String& text) noexcept
	:
	ControlBase(text)
{
	_parent = parent;
}

ControlBase::ControlBase(ControlBase* parent, const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
	:
	ControlBase(text, left, top, width, height)
{
	_parent = parent;
}

ControlBase::~ControlBase()
{
	if (_impl && !GetState(Flags::BackendDestroyed))
	{
		DestroyWindowBackend(_impl);
		SetState(Flags::BackendDestroyed, true);
		for (auto c : _controls)
		{
			delete c;
		}
	}

	_impl = nullptr;
}

void ControlBase::Initialize(InitializationContext& ctx)
{
	_uiContext = ctx.UIContext;
	_parent = ctx.Parent;
	if (!_font) _font = ctx.UIContext->GetDefaultFont();
	InitializeImpl(this, ctx);
}

Boolean ControlBase::IsHandledCreated() const noexcept
{
	return _impl != nullptr;
}

void ControlBase::Close()
{
	if (!_impl || GetState(Flags::BackendDestroyed))
		return;

	// Isso dispara WM_CLOSE → WM_DESTROY
	DestroyWindowBackend(_impl);
}

Window* ControlBase::FindWindow() noexcept
{
	ControlBase* c = UIHandle(this);

	while (c->_parent != nullptr)
		c = c->_parent;

	return (Window*)c;
}

void ControlBase::EnableResize()
{
	if (GetState(Flags::Resizable)) return;

	SetState(Flags::Resizable, true);
	return SetWindowResize(_impl);
}

void ControlBase::DisableResize()
{
	if (!GetState(Flags::Resizable)) return;

	SetState(Flags::Resizable, false);
	return SetWindowResize(_impl);
}

void ControlBase::AddControl(ControlBase* control)
{
	if (!control) return;

	control->_parent = UIHandle(this);
	_controls.Add(control);
}

void ControlBase::RemoveControl(ControlBase* control)
{
	if (!control) return;

	_controls.Remove(control);
	delete control;
}

void ControlBase::SetEnabled(Boolean enabled)
{
	SetEnabledControlBackend(_impl, enabled);
}

void ControlBase::Show()
{
	if (!GetState(Flags::Visible)) 
	{
		ShowControlBackend(_impl);
		SetState(Flags::Visible, true);
	}
}

void ControlBase::Hide()
{
	if (GetState(Flags::Visible))
	{
		HideControlBackend(_impl);
		SetState(Flags::Visible, false);
	}
}
void ControlBase::Update()
{
	UpdateControlBackend(_impl);
}

void ControlBase::Enable() noexcept
{
	if (!GetState(Flags::Enabled))
	{
		NB_Enable(_impl);
		SetState(Flags::Enabled, true);
		//OnEnableChange call
	}
}

void ControlBase::Disable() noexcept
{
	if (GetState(Flags::Enabled))
	{
		NB_Disable(_impl);
		SetState(Flags::Enabled, false);
		//OnEnabledChange call
	}
}

Boolean ControlBase::OnEvent(Event& e)
{
	for (auto c : _controls)
	{
		if (c)
			c->OnEvent(e);
	}

	auto category = (uint8_t)e.GetCategory();

	if (category >= CountOf(CategoryTables))
		return false;

	const auto& categoryTable = CategoryTables[category];

	auto type = e.GetType();

	if (type >= categoryTable.Count)
		return false;

	auto handler = categoryTable.Handlers[type];

	if (handler)
	{
		(this->*handler)(e);
	}

	return false;
}

void ControlBase::SetX(i32 x) noexcept
{
	SetLocation({ x, GetY() });
}

void ControlBase::SetY(i32 y) noexcept
{
	SetLocation({ GetX(), y });
}

void ControlBase::SetSize(i32 x, i32 y) noexcept
{
	SetSize({ x, y });
}

void ControlBase::SetSize(Size s) noexcept
{
	if (GetState(Flags::AutoSize)) return;

	_width = s.GetWidth();
	_height = s.GetHeight();

	if (IsHandledCreated())
	{
		ResizeControl(_impl);
	}
}

void ControlBase::SetWidth(i32 width) noexcept
{
	SetSize({ width, GetHeight() });
}

void ControlBase::SetHeight(i32 height) noexcept
{
	SetSize({ GetWidth(), height });
}

void ControlBase::SetFont(Font* font) noexcept
{
	if (IsHandledCreated())
	{
		FontChangedEvent ev = FontChangedEvent(UIHandle(this), font, _font);
		OnEvent(ev);

		// User intercepted and cancelled font chang event
		if (ev.Has(EventFlags::Handled))
		{
			return;
		}

		if (_font == font) return;
		_font = font;

		if (GetState(Flags::AutoSize))
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

void ControlBase::RedrawWithSize(Size s) noexcept
{
	_width = s.GetWidth();
	_height = s.GetHeight();

	if (IsHandledCreated())
	{
		ResizeControl(_impl);
	}
}