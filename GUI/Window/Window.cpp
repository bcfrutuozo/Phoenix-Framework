#include "Window.hpp"
#include "GUI/Core/Control.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "GUI/Rendering/Vulkan/VulkanContext.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/Input/KeyboardEvents.hpp"
#include "Events/GUI/BehavioralEvents.hpp"
#include "System/Globals.hpp"

Window::Window() noexcept
{

}

Window::Window(const String& title, i32 width, i32 height, Boolean isResizable) noexcept
	:
	Window(title, (Phoenix::ScreenWidth / 2) - (width /2), (Phoenix::ScreenHeight / 2) - (height / 2), width, height, isResizable)
{

}

Window::Window(const String& title, i32 x, i32 y, i32 width, i32 height, Boolean isResizable) noexcept
	:
	Control(title, x, y, width, height)
{
	_resizable = isResizable;
}

Window::~Window()
{

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
	_uiContext = ctx.UIContext;
	_parent = ctx.Parent;
	if (!_font) _font = ctx.UIContext->GetDefaultFont();
	_impl = CreateWindowBackend(this, GetParentBackend(), ctx.Queue, ctx.UIContext);
	if (_vk) _vk->Initialize();
	
	for (auto c : _controls)
	{
		InitializationContext cCtx;
		cCtx.Font = _font;
		cCtx.UIContext = _uiContext;
		cCtx.Parent = this;
		c->Initialize(cCtx);
	}
}

Boolean Window::Dispatch(Event& e)
{
	if (dispatch_to_controls(e))
		return true;

	EventDispatcher d(e);

	// After dispatching for all children controls first, tries to handle event on Window
	return OnEvent(e);
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