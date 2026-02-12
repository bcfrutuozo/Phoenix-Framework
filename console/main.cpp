#include "Application/Phoenix.hpp"

static void ChangeWindowSize(ResizingEvent* e)
{
    printf("Resize: %u x %u\n", e->Width, e->Height);
    Console::WriteLine(e->Handle.AsWindow()->GetText());
}

static void WindowClosed()
{
    printf("Window closed\n");
}

static void TesteLabel(MouseButtonDownEvent* b)
{
    Control* c = b->Target.AsControl();

    b->Clear(EventFlags::Consumed);

    if (b->Button == MouseButton::Right)
    {
        c->SetWidth(c->GetWidth() - 10);
    }
    else if(b->Button == MouseButton::Left)
    {
        c->SetWidth(c->GetWidth() + 10);
    }
}

static void ChangeFont(FontChangedEvent* e)
{
    auto f1 = e->NewFont;
    auto f2 = e->PreviousFont;

    bool c = f1 == f2;
}

int main(int argc, char* argv[])
{
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
    TextBox* tb = new TextBox("Teste", 10, 10);
    tb->SetBackgroundColor(Color::Black);
    tb->SetForeColor(Color::White);
    Font f = Font("Elephant", 30, 200);
    l->DisableAutoSize();
    l->SetTextFormat(TextFormat::WordWrap);
    l->SetBackgroundColor(Color::Aquamarine);
    l->SetSize({ 50, 400 });
    l->SetFont(&f);
    l->OnFontChanged = ChangeFont;
    l->OnMouseDown = TesteLabel;
    app->Attach(w2);
    w2->AddControl(l);
    w2->AddControl(l2);
    w2->AddControl(tb);
    
    app->Attach(w);
    
    app->Run();
    delete app;
    return 0;
}