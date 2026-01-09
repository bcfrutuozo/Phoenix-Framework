#include "Window.hpp"
#include "WindowBackend.hpp"
#include "GUI/Rendering/Vulkan/VulkanContext.hpp"
#include "System/Events/EventDispatcher.hpp"
#include "GUI/Events/UIEvents.hpp"
#include "GUI/Core/Control.hpp"

Window::Window(const WindowDesc& desc)
	:
	_controls(16)
{
	_impl = CreateWindowBackend(this, desc);
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

UIHandle Window::GetNativeHandle() const noexcept
{
	return GetWindowBackendHandle(_impl);
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

void Window::Dispatch(Event& e)
{
	dispatch_to_controls(e);

	EventDispatcher d(e);

	// ------------------------------------------------------------
	// Close / destroy
	// ------------------------------------------------------------
	d.Dispatch<UICloseEvent>(
		EventCategory::UI,
		UIEventType::Close,
		[&](const UICloseEvent& ev)
		{
			if (ev.Handle != GetNativeHandle())
				return;

			// 🔔 Notifica o usuário
			if (OnClose)
				OnClose();

			// 🔑 DECISÃO LOCAL:
			// Por padrão, fecha a janela
			// (política simples e previsível)
			//Close(); // chama DestroyWindow(hwnd)
		}
	);

	d.Dispatch<UIDestroyEvent>(
		EventCategory::UI,
		UIEventType::Destroy,
		[&](const UIDestroyEvent& ev)
		{
			if (ev.Handle != GetNativeHandle())
				return;

			_backendDestroyed = true;

			// 🔔 Notifica
			if (OnDestroy)
				OnDestroy();

			// ❌ NÃO chama DestroyWindow aqui
			// ❌ NÃO decide política global
			// Só cleanup local
		}
	);

	// ------------------------------------------------------------
	// Resize / move
	// ------------------------------------------------------------
	d.Dispatch<UIResizeEvent>(
		EventCategory::UI, UIEventType::Resize,
		[&](const UIResizeEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnResize)
				OnResize(ev.Width, ev.Height);
		}
	);

	d.Dispatch<UIMoveEvent>(
		EventCategory::UI, UIEventType::Move,
		[&](const UIMoveEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnMove)
				OnMove(ev.X, ev.Y);
		}
	);

	// ------------------------------------------------------------
	// Min / max
	// ------------------------------------------------------------
	d.Dispatch<UIMinimizeEvent>(
		EventCategory::UI, UIEventType::Minimize,
		[&](const UIMinimizeEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnMinimize)
				OnMinimize();
		}
	);

	d.Dispatch<UIMaximizeEvent>(
		EventCategory::UI, UIEventType::Maximize,
		[&](const UIMaximizeEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnMaximize)
				OnMaximize();
		}
	);

	// ------------------------------------------------------------
	// Visibility / focus
	// ------------------------------------------------------------
	d.Dispatch<UIShowEvent>(
		EventCategory::UI, UIEventType::Show,
		[&](const UIShowEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnShow)
				OnShow();
		}
	);

	d.Dispatch<UIHideEvent>(
		EventCategory::UI, UIEventType::Hide,
		[&](const UIHideEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnHide)
				OnHide();
		}
	);

	d.Dispatch<UIFocusGainedEvent>(
		EventCategory::UI, UIEventType::FocusGained,
		[&](const UIFocusGainedEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnFocusGained)
				OnFocusGained();
		}
	);

	d.Dispatch<UIFocusLostEvent>(
		EventCategory::UI, UIEventType::FocusLost,
		[&](const UIFocusLostEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnFocusLost)
				OnFocusLost();
		}
	);

	// ------------------------------------------------------------
	// DPI
	// ------------------------------------------------------------
	d.Dispatch<UIDPIChangedEvent>(
		EventCategory::UI, UIEventType::DPIChanged,
		[&](const UIDPIChangedEvent& ev)
		{
			if (ev.Handle == GetNativeHandle() && OnDpiChanged)
				OnDpiChanged(ev.DPI);
		}
	);
}

void Window::AddControl(Control* control)
{
	control->AttachTo(this);
	_controls.Add(control);
}

void Window::attach_event_queue(EventQueue* queue)
{
	AttachEventQueue(_impl, queue);
	for (const auto& c : _controls)
		c->AttachEventQueue(queue);
}

void Window::dispatch_to_controls(Event& e)
{
	if (e.Is(EventCategory::Mouse) || e.Is(EventCategory::Keyboard) || e.Is(EventCategory::Text))
	{
		const UIHandle* target = nullptr;

		if (e.Is(EventCategory::Mouse))
			target = &e.As<MouseButtonDownEvent>().Handle; // ou ButtonDown/Up
		//else if (e.Is(EventCategory::Keyboard))
		//    target = &e.As<KeyDownEvent>().Handle;
		//else if (e.Is(EventCategory::Text))
		//    target = &e.As<TextInputEvent>().Handle;

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