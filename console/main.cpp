#include "system/Framework.hpp"
#include "window/Window.hpp"
#include "render/vulkan/VulkanContext.hpp"

#include <cassert>
#include <string>

int main(int argc, char* argv[])
{
    WindowDesc aa;
    aa.height = 800;
    aa.width = 800;
    aa.title = "Teste";
    aa.resizable = true;

    EventQueue queue;
    queue.Clear();

    EventQueue queue2;
    queue2.Clear();

    Window window(aa, queue);
    window.Show();

    WindowDesc ab;
    ab.height = 500;
    ab.width = 500;
    ab.title = "Teste 2";
    ab.resizable = false;

    VulkanContext vk(window);

    Window win2(ab, queue2);
    win2.Show();

    // 5) Loop mínimo
    bool running = true;

    while (running)
    {
        // bombeia mensagens do SO
        window.PollEvents();
        vk.RenderFrame();

        // consome eventos da engine
        Event ev;
        while (queue.Poll(ev))
        {
            switch (ev.type)
            {
            case EventType::WindowCloseRequested:
            {
                auto& we = static_cast<Event&>(ev);

                if (we.type == EventType::WindowCloseRequested)
                {
                    running = false;
                }
                break;
            }
            default:
                break;
            }
        }

        // aqui entraria render / update
    }

    str z = "AB👍C👍👍👍👍👍";
    c8 c = 'A';
    Int32 zz = 4;

    Pointer aaaa(&z);

    char x = c;
    bool ok = (c == 'A');

    CodePoint cp = 0x00E1;
    char32_t u = cp;
    bool ok2 = (cp == 0x00E1);
}