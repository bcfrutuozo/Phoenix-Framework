#include "Platform/Phoenix.hpp"

static void ChangeWindowSize(ResizingEvent* e)
{
    printf("Resize: %u x %u\n", e->Width, e->Height);
    e->Handled = false;
    Console::WriteLine(e->Handle.AsWindow()->GetText());
}

static void WindowClosed()
{
    printf("Window closed\n");
}

static void TesteLabel(MouseButtonDownEvent* b)
{
    if (b->Button == MouseButton::Left)
        printf("Cliquei LEFT!");
    else
        printf("Cliquei outro!");
    b->Handled = true;
}

int main(int argc, char* argv[])
{
    GUIApplication* app = CreateGUIApplication();
    Window* w = new Window("ABC", 800, 600);
    w->OnResizing = ChangeWindowSize;
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