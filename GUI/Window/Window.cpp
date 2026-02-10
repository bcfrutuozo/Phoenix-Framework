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
		for (auto c : _controls)
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
	// Set font if none was specified
	_desc->UIContext = ctx.UIContext;
	if (!_desc->Font) _desc->Font = ctx.UIContext->GetDefaultFont();

	_impl = CreateWindowBackend(this, _desc);
	if (_vk) _vk->Initialize();
	AttachEventQueue(_impl, ctx.Queue);
	
	for (auto c : _controls)
	{
		Control::InitializationContext cCtx;
		cCtx.Font = _desc->Font;
		cCtx.UIContext = _desc->UIContext;
		cCtx.WindowBackend = _impl;
		c->Initialize(cCtx);
	}
}

Boolean Window::Dispatch(Event& e)
{
	if (dispatch_to_controls(e))
		return true;

	EventDispatcher d(e);

	// ------------------------------------------------------------
	// Close / destroy
	// ------------------------------------------------------------
	if (d.Dispatch<CloseEvent>(
		EventCategory::UI,
		UIEventType::Close,
		[&](CloseEvent& ev)
		{
			if (ev.Handle != UIHandle::FromWindow(this))
				return;

			// 🔔 Notifica o usuário
			if (OnClose)
				OnClose(&ev);
		}
	)) return true;

	if(d.Dispatch<DestroyEvent>(
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
	)) return true;

	// ------------------------------------------------------------
	// Resize / move
	// ------------------------------------------------------------
	if(d.Dispatch<ResizedEvent>(
		EventCategory::UI, UIEventType::Resized,
		[&](ResizedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnResized)
				OnResized(&ev);
		}
	)) return true;

	if(d.Dispatch<ResizingEvent>(
		EventCategory::UI, UIEventType::Resizing,
		[&](ResizingEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnResizing)
				OnResizing(&ev);
		}
	)) return true;

	if(d.Dispatch<MoveEvent>(
		EventCategory::UI, UIEventType::Move,
		[&](MoveEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMove)
				OnMove(&ev, ev.X, ev.Y);
		}
	)) return true;

	// ------------------------------------------------------------
	// Min / max
	// ------------------------------------------------------------
	if(d.Dispatch<MinimizeEvent>(
		EventCategory::UI, UIEventType::Minimize,
		[&](MinimizeEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMinimize)
				OnMinimize(&ev);
		}
	)) return true;

	if(d.Dispatch<MaximizeEvent>(
		EventCategory::UI, UIEventType::Maximize,
		[&](MaximizeEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnMaximize)
				OnMaximize(&ev);
		}
	)) return true;

	// ------------------------------------------------------------
	// Visibility / focus
	// ------------------------------------------------------------
	if(d.Dispatch<RestoreEvent>(
		EventCategory::UI, UIEventType::Restore,
		[&](RestoreEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnRestore)
				OnRestore(&ev);
		}
	)) return true;

	if(d.Dispatch<ShowEvent>(
		EventCategory::UI, UIEventType::Show,
		[&](ShowEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnShow)
				OnShow(&ev);
		}
	)) return true;

	if(d.Dispatch<HideEvent>(
		EventCategory::UI, UIEventType::Hide,
		[&](HideEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnHide)
				OnHide(&ev);
		}
	)) return true;

	if(d.Dispatch<FocusGainedEvent>(
		EventCategory::UI, UIEventType::FocusGained,
		[&](FocusGainedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnFocusGained)
				OnFocusGained(&ev);
		}
	)) return true;

	if(d.Dispatch<FocusLostEvent>(
		EventCategory::UI, UIEventType::FocusLost,
		[&](FocusLostEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnFocusLost)
				OnFocusLost(&ev);
		}
	)) return true;

	// ------------------------------------------------------------
	// DPI
	// ------------------------------------------------------------
	if(d.Dispatch<DPIChangedEvent>(
		EventCategory::UI, UIEventType::DPIChanged,
		[&](DPIChangedEvent& ev)
		{
			if (ev.Handle == UIHandle::FromWindow(this) && OnDPIChanged)
				OnDPIChanged(&ev);
		}
	)) return true;

	// ------------------------------------------------------------
	// FontChanged
	// ------------------------------------------------------------
	if(d.Dispatch<FontChangedEvent>(
		EventCategory::UI, UIEventType::FontChanged,
		[&](FontChangedEvent& ev)
		{
			if (ev.Target == UIHandle::FromWindow(this) && OnFontChanged)
				OnFontChanged(&ev);
		}
	)) return true;

	return false;
}

void Window::AddControl(Control* control)
{
	control->AttachTo(this);
	_controls.Add(control);
}

void Window::SetFont(Font* font) noexcept
{
	_desc->Font = font;
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

bool Window::dispatch_to_controls(Event& e)
{
	for (auto& c : _controls)
	{
		if(c->OnEvent(e));
			return true;
	}

	return false;
}