#pragma once

#include "GUI/Core/Control.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "Events/EventQueue.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "GUI/Drawing/Font.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "GUI/Core/HorizontalAlignment.hpp"
#include "GUI/Core/VerticalAlignment.hpp"
#include "GUI/Core/TextFormat.hpp"
#include "GUI/Context/UIContext.hpp"

class VulkanContext;

class Window : public Control
{
public:

    friend class MessageBox;

    Window() noexcept;
    Window(const String& title, i32 width, i32 height, Boolean isResizable = true) noexcept;
    Window(const String& title, i32 x, i32 y, i32 width, i32 height, Boolean isResizable = true) noexcept;
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Boolean Dispatch(Event& e);

    SurfaceHandle GetSurfaceHandle() const noexcept;

    void AttachRenderContext(VulkanContext* ctx);
    VulkanContext* GetRenderContext() const;

    void Initialize(InitializationContext ctx) override;

private:

    bool dispatch_to_controls(Event& e); 
    VulkanContext* _vk = nullptr;
};