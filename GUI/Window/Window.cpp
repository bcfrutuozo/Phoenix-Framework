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
	SetState(Flags::Resizable, isResizable);
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

void Window::InitializeImpl(InitializationContext& ctx)
{
	// Set font if none was specified
	_impl = CreateWindowBackend(this, GetParentBackend(), ctx.Queue, ctx.UIContext, ctx.EventSink);
	SetState(Flags::Created, true);
	if (_vk) _vk->Initialize();
	
	for (auto c : _controls)
	{
		InitializationContext cCtx;
		cCtx.Font = _font;
		cCtx.UIContext = _uiContext;
		cCtx.Parent = (Control*)this;
		cCtx.EventSink = ctx.EventSink;
		c->Initialize(cCtx);
	}
}