#include "visef.h"
#include <bgfx/bgfx.h>
#include <bx/timer.h>
#include <new>
#include "event_queue.h"

namespace visef
{
    extern bool nextEvent(Event&);

    App::App() :
        m_lastDeltaTime(0.f),
        m_timeSinceStart(0.0),
        m_quit(false)
    {

    }

    App::~App()
    {

    }

    void App::quit()
    {
        m_quit = true;
    }

    void App::run()
    {
        bgfx::init();

        int64_t lastTime = bx::getHPCounter();
        int64_t currentTime = 0;

        while (!processEvents() && !m_quit)
        {
            currentTime = bx::getHPCounter();
            const int64_t time = currentTime - lastTime;
            const double frequency = (double)bx::getHPFrequency();

            m_lastDeltaTime = float(time * (1.0 / frequency));
            m_timeSinceStart += m_lastDeltaTime;

            bgfx::setViewClear(0,
                BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                0x303030ff, // clear color
                1.0f, // depth
                0 // stencil
                );

            // TODO get width & height some other way
            bgfx::setViewRect(0, 0, 0, uint16_t(1280), uint16_t(720));

            bgfx::touch(0);

            bgfx::frame();
        }

        bgfx::shutdown();
    }

    bool App::processEvents()
    {
        bool exit = false;

        Event ev;
        while (nextEvent(ev))
        {
            switch (ev.m_type)
            {
            case EventType::Exit:
                exit = true;
                break;

            default:
                break;
            }
        }

        return exit;
    }

    char mem[sizeof(App)];
    static App* s_app = NULL;

    void run()
    {
        // TODO: assert
        if (s_app != NULL) return;

        s_app = new (mem)App;
        s_app->run();
        s_app->~App();
        s_app = NULL;
    }

    App* app()
    {
        return s_app;
    }
}