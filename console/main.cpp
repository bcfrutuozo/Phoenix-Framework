#include "system/Framework.hpp"
#include "window/Window.hpp"
#include "render/vulkan/VulkanContext.hpp"

#include <cassert>
#include <string>

int main(int argc, char* argv[])
{
    AppKind kind = AppKind::Windowed;
    GUIApplication* app = CreateGUIApplication(kind);
    EventQueue qu;
    qu.Clear();
    Window* w = new Window({ "ABC", 800, 600 }, qu);
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