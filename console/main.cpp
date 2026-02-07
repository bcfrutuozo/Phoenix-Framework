#include "Application/Phoenix.hpp"

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
    String aa = "BC";
    String cc = "CDE";

    Boolean zz = aa == cc;

    u32 a = 2;
    const char* c = "ABCDE";
    Console::WriteLine(c[a]);
    
    GUIApplication* app = CreateGUIApplication();
    Window* w = new Window("ABC", 800, 600);
    w->OnResizing = ChangeWindowSize;
    VulkanContext* cx = new VulkanContext(w);
    w->AttachRenderContext(cx);
    Window* w2 = new Window( "Teste", 1000, 1000 );
    Label* l = new Label("Teste Label 1", 50, 50);
    Label* l2 = new Label("Teste Label 2", 100, 300);
    l2->SetFont(new Font("Vivaldi Italic", 25, 150));
    Font f = Font("Elephant", 30, 200);
    l->SetFont(&f);
    l->OnMouseDown = TesteLabel;
    app->Attach(w2);
    w2->AddControl(l);
    w2->AddControl(l2);
    
    app->Attach(w);
    
    app->Run();
    delete app;
    return 0;
}