#include "System/Framework.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Window/Label.hpp"
#include "GUI/Rendering/Vulkan/VulkanContext.hpp"
#include "Events/Categories/MouseEvents.hpp"


#include <cassert>
#include <string>

void ChangeWindowSize(u32 w, u32 h)
{
    printf("Resize: %u x %u\n", w, h);
}

void WindowClosed()
{
    printf("Window closed\n");
}

void TesteLabel(MouseButton b)
{
    if (b == MouseButton::Left)
        printf("Cliquei LEFT!");
    else
        printf("Cliquei outro!");
}

int main(int argc, char* argv[])
{
    AppKind kind = AppKind::Windowed;
    GUIApplication* app = CreateGUIApplication(kind);
    Window* w = new Window({"ABC", 800, 600 });
    
    w->OnResize = ChangeWindowSize;
    VulkanContext* cx = new VulkanContext(*w);
    w->AttachRenderContext(cx);

    Window* w2 = new Window({ "Teste", 1000, 1000 });
    Label* l = new Label("Teste Label: ", 50, 50);
    //l->OnMouseDown = TesteLabel;
    w2->AddControl(l);

    w2->AttachRenderContext(cx);

    app->Attach(w);
    app->Attach(w2);
    app->Run();
    delete app;
    return 0;
}