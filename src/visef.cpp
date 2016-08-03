#include "visef.h"
#include <bgfx/bgfx.h>
#include <bx/timer.h>
#include <new>

namespace visef
{

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

        while (!m_quit)
        {
            currentTime = bx::getHPCounter();
            const int64_t time = currentTime - lastTime;
            const double frequency = (double)bx::getHPFrequency();

            m_lastDeltaTime = float(time * (1.0 / frequency));
            m_timeSinceStart += m_lastDeltaTime;

            bgfx::frame();
        }

        bgfx::shutdown();
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