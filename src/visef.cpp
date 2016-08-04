#include "visef.h"
#include <bgfx/bgfx.h>
#include <bx/timer.h>
#include <new>
#include <stdio.h>
#include <inttypes.h> // PRIxYY
#include "event_queue.h"
#include "hid/input.h"
#include "error/error.h"

namespace visef
{
    extern bool nextEvent(Event&);

    // implemented in demo.cpp
    extern void demoInit();
    extern void update(float dt);
    extern void render(float dt);

    App::App() :
        m_lastDeltaTime(0.f),
        m_timeSinceStart(0.0),
        m_width(0),
        m_height(0),
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

        bgfx::setDebug(BGFX_DEBUG_TEXT);

        demoInit();

        int64_t lastTime = bx::getHPCounter();
        int64_t currentTime = 0;

        while (!processEvents() && !m_quit)
        {
            currentTime = bx::getHPCounter();
            const int64_t time = currentTime - lastTime;
            const double frequency = (double)bx::getHPFrequency();

            m_lastDeltaTime = float(time * (1.0 / frequency));
            m_timeSinceStart += m_lastDeltaTime;
            bgfx::dbgTextClear();
            {
                const int64_t t0 = bx::getHPCounter();
                update(m_lastDeltaTime);
                const int64_t t1 = bx::getHPCounter();
                bgfx::dbgTextPrintf(0, 1, 0x4f, "Frame update: %7.3f[ms]", (t1 - t0) * (1000.0 / frequency));
            }
            {
                const int64_t t0 = bx::getHPCounter();
                render(m_lastDeltaTime);
                const int64_t t1 = bx::getHPCounter();
                bgfx::dbgTextPrintf(0, 2, 0x6f, "Frame draw:   %7.3f[ms]", (t1 - t0) * (1000.0 / frequency));
            }
            m_input.update();

            bgfx::frame();
        }

        bgfx::shutdown();
    }

    bool App::processEvents()
    {
        bool exit = false;
        bool reset = false;

        Event ev;
        while (nextEvent(ev))
        {
            switch (ev.m_type)
            {
            case EventType::Exit:
                exit = true;
                break;

            case EventType::Resolution:
            {
                ResolutionEvent& res = ev.m_resolution;

                fprintf(stderr, "Changing resolution to %" PRIu16 "x%" PRIu16 "\n", res.m_width, res.m_height);

                m_width = res.m_width;
                m_height = res.m_height;
                reset = true;
            }
            break;

            case EventType::Button:
            {
                ButtonEvent& btn = ev.m_button;

                switch (btn.m_device)
                {
                case InputDeviceType::Keyboard:
                    m_input.keyboard().setKeyState(btn.m_button, btn.m_button > 0);
                    break;
                }
            }
            break;

            default:
                break;
            }
        }

        if (reset)
        {
            // reset backbuffer
            bgfx::reset(m_width, m_height, BGFX_RESET_NONE);
        }

        return exit;
    }

    char mem[sizeof(App)];
    static App* s_app = NULL;

    void run()
    {
        VE_ASSERT(s_app == NULL, "App already initialized");

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