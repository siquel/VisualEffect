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
            bgfx::setViewRect(0, 0, 0, m_width, m_height);

            bgfx::touch(0);

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
                    if (btn.m_pressed)
                    {
                        fprintf(stderr, "Key \"%s\" is down\n", getKeyName(btn.m_button));
                    }
                    else
                    {
                        fprintf(stderr, "Key \"%s\" is up\n", getKeyName(btn.m_button));
                    }
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