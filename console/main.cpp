#include "Platform/Phoenix.hpp"

void ChangeWindowSize(Window* window, u32 w, u32 h)
{
    printf("Resize: %u x %u\n", w, h);
    Console::WriteLine(window->GetText());
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
    GUIApplication* app = CreateGUIApplication();
    Window* w = new Window("ABC", 800, 600);
    w->OnResize = ChangeWindowSize;
    VulkanContext* cx = new VulkanContext(w);
    w->AttachRenderContext(cx);
    Window* w2 = new Window( "Teste", 1000, 1000 );
    Label* l = new Label("Teste Label: ", 50, 50);
    l->OnMouseDown = TesteLabel;
    app->Attach(w2);
    w2->AddControl(l);

    app->Attach(w);

    app->Run();
    delete app;
    return 0;
}