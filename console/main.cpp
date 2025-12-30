#include "system/Framework.hpp"
#include "window/Window.hpp"
#include "render/vulkan/VulkanContext.hpp"

#include <cassert>
#include <string>

void ChangeWindowSize(uint32_t w, uint32_t h)
{
    printf("Resize: %u x %u\n", w, h);
}

void WindowClosed()
{
    printf("Window closed\n");
}

int main(int argc, char* argv[])
{
    AppKind kind = AppKind::Windowed;
    GUIApplication* app = CreateGUIApplication(kind);
    EventQueue qu;
    qu.Clear();
    Window* w = new Window({ "ABC", 800, 600 }, qu);
    w->OnResize = ChangeWindowSize;
    VulkanContext* cx = new VulkanContext(*w);
    w->AttachRenderContext(cx);

    Window* w2 = new Window({ "Teste", 1000, 1000 }, qu);
    w2->AttachRenderContext(cx);

    app->Attach(w);
    app->Attach(w2);
    app->Run();
    delete app;
    return 0;
}