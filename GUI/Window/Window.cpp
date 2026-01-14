#include "Window.hpp"
#include "WindowBackend.hpp"
#include "GUI/Rendering/Vulkan/VulkanContext.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/Input/KeyboardEvents.hpp"
#include "Events/GUI/BehavioralEvents.hpp"
#include "GUI/Core/Control.hpp"
#include "System/Globals.hpp"

Window::Window(const String& title, i32 width, i32 height, Boolean isResizable)
	:
	Window(title, width, height, (Phoenix::ScreenWidth / 2) - (width /2), (Phoenix::ScreenHeight / 2) - (height / 2), isResizable)
{

}

Window::Window(const String& title, i32 width, i32 height, i32 x, i32 y, Boolean isResizable)
{
	_desc = new WindowDescriptor();
	_desc->Title = title; 
	_desc->Size = Size(width, height);
	_desc->Resizable = isResizable;
	_desc->Location = Point(x, y);
}

Window::~Window()
{
	if (_impl && !_backendDestroyed)
	{
		DestroyWindowBackend(_impl);
		for (const auto& c : _controls)
		{
			delete c;
		}
	}

	delete _desc;
	_desc = nullptr;
	_impl = nullptr;
}

void Window::Show()
{
	ShowWindowBackend(_impl);
}

void Window::Close()
{
	if (!_impl || _backendDestroyed)
		return;

	// Isso dispara WM_CLOSE → WM_DESTROY
	DestroyWindowBackend(_impl);
}

void Window::Update()
{
	if (!_impl || _backendDestroyed)
		return;

	// Isso dispara WM_CLOSE → WM_DESTROY
	UpdateWindowBackend(_impl);
}

void Window::PollEvents()
{
	PollWindowBackendEvents(_impl);
}

SurfaceHandle Window::GetSurfaceHandle() const noexcept
{
	return GetWindowSurfaceHandle(_impl);
}

void Window::AttachRenderContext(VulkanContext* ctx)
{
	_vk = ctx;
}

VulkanContext* Window::GetRenderContext() const
{
	return _vk;
}

void Window::Initialize(InitializationContext ctx)
{
	_impl = CreateWindowBackend(this, _desc);
	if (_vk) _vk->Initialize();
	AttachEventQueue(_impl, ctx.queue);
	for (const auto& c : _controls)
	{
		Control::InitializationContext cCtx;
		cCtx.WindowBackend = _impl;
		c->Initialize(cCtx);
	}
}

void Window::Dispatch(Event& e)
{
	dispatch_to_controls(e);

	EventDispatcher d(e);

	// ------------------------------------------------------------
	// Close / destroy
	// ------------------------------------------------------------
	d.Dispatch<CloseEvent>(
		EventCategory::UI,
		UIEventType::Close,
		[&](CloseEvent& ev)
		{
			if (ev.Handle != UIHandle::FromWindow(this))
				return;

			// 🔔 Notifica o usuário
			if (OnClose)
				OnClose(&ev);

			// 🔑 DECISÃO LOCAL:
			// Por padrão, fecha a janela
			// (política simples e previsível)
		}
	);

	d.Dispatch<DestroyEvent>(
		EventCategory::UI,
		UIEventType::Destroy,
		[&](DestroyEvent& ev)
		{
			if (ev.Handle != UIHandle::FromWindow(this))
				return;

			_backendDestroyed = true;

			// 🔔 Notifica
			if (OnDestroy)
				OnDestroy(&ev);

			// ❌ NÃO chama DestroyWindow aqui
			// ❌ NÃO decide política global
			// Só cleanup local
		}
	);

	// ------------------------------------------------------------
	// Resize / move
	// ------------------------------------------------------------
	d.Dispatch<ResizedEvent>(
		EventCategory::UI, UIEventType::Resized,
		[&](ResizedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnResized)
				OnResized(&ev);
		}
	);

	d.Dispatch<ResizingEvent>(
		EventCategory::UI, UIEventType::Resizing,
		[&](ResizingEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnResizing)
				OnResizing(&ev);
		}
	);

	d.Dispatch<MoveEvent>(
		EventCategory::UI, UIEventType::Move,
		[&](MoveEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMove)
				OnMove(&ev, ev.X, ev.Y);
		}
	);

	// ------------------------------------------------------------
	// Min / max
	// ------------------------------------------------------------
	d.Dispatch<MinimizeEvent>(
		EventCategory::UI, UIEventType::Minimize,
		[&](MinimizeEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMinimize)
				OnMinimize(&ev);
		}
	);

	d.Dispatch<MaximizeEvent>(
		EventCategory::UI, UIEventType::Maximize,
		[&](MaximizeEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMaximize)
				OnMaximize(&ev);
		}
	);

	// ------------------------------------------------------------
	// Visibility / focus
	// ------------------------------------------------------------
	d.Dispatch<RestoreEvent>(
		EventCategory::UI, UIEventType::Restore,
		[&](RestoreEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnRestore)
				OnRestore(&ev);
		}
	);

	d.Dispatch<ShowEvent>(
		EventCategory::UI, UIEventType::Show,
		[&](ShowEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnShow)
				OnShow(&ev);
		}
	);

	d.Dispatch<HideEvent>(
		EventCategory::UI, UIEventType::Hide,
		[&](HideEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnHide)
				OnHide(&ev);
		}
	);

	d.Dispatch<FocusGainedEvent>(
		EventCategory::UI, UIEventType::FocusGained,
		[&](FocusGainedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnFocusGained)
				OnFocusGained(&ev);
		}
	);

	d.Dispatch<FocusLostEvent>(
		EventCategory::UI, UIEventType::FocusLost,
		[&](FocusLostEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnFocusLost)
				OnFocusLost(&ev);
		}
	);

	// ------------------------------------------------------------
	// DPI
	// ------------------------------------------------------------
	d.Dispatch<DPIChangedEvent>(
		EventCategory::UI, UIEventType::DPIChanged,
		[&](DPIChangedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnDPIChanged)
				OnDPIChanged(&ev);
		}
	);
}

void Window::AddControl(Control* control)
{
	control->AttachTo(this);
	_controls.Add(control);
}

void Window::SetText(const String& text) const
{
	_desc->Title = text;
	return SetWindowTitle(_impl, _desc);
}

void Window::SetSize(i32 width, i32 height) const
{
	_desc->Size = Size(width, height);
	return SetWindowSize(_impl, _desc);
}

void Window::SetLocation(i32 x, i32 y) const
{
	_desc->Location = Point(x, y);
	return SetWindowLocation(_impl, _desc);
}

void Window::EnableResize()
{
	if (IsResizable()) return;
	
	_desc->Resizable = true;
	return SetWindowResize(_impl, _desc);
}

void Window::DisableResize()
{
	if (!IsResizable()) return;

	_desc->Resizable = false;
	return SetWindowResize(_impl, _desc);
}

void Window::dispatch_to_controls(Event& e)
{
	if (e.Is(EventCategory::Mouse) || e.Is(EventCategory::Keyboard) || e.Is(EventCategory::Text))
	{
		const UIHandle* target = nullptr;

		if (e.Is(EventCategory::Mouse))
		{
			target = &e.As<MouseButtonDownEvent>().Target; // ou ButtonDown/Up
			if (target == nullptr) target = &e.As<MouseButtonUpEvent>().Target; // ou ButtonDown/Up
		}
		else if (e.Is(EventCategory::Keyboard))
		{
			target = &e.As<KeyDownEvent>().Target;
			if (target == nullptr) target = &e.As<KeyUpEvent>().Target;
		}
		else if (e.Is(EventCategory::Text))
		{
			target = &e.As<TextInputEvent>().Target;
			if (target == nullptr) target = &e.As<ImeCompositionEvent>().Target;
		}
		if (target && target->IsControl())
		{
			if (Control* c = target->AsControl())
			{
				c->OnEvent(e);
				if (e.Handled)
					return;
			}
		}
	}
}