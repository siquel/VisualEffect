#pragma once
#include <stdint.h>

namespace visef
{
    struct App
    {
        App();

        ~App();

        void run();

        void quit();

        bool processEvents();

    private:
        float m_lastDeltaTime; // in seconds
        double m_timeSinceStart; // in seconds

        uint16_t m_width;
        uint16_t m_height;

        bool m_quit; // should we exit
    };

    void run();

    App* app();
}