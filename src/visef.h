#pragma once
#include <stdint.h>
#include "hid/input_manager.h"

namespace visef
{
    struct App
    {
        App();

        ~App();

        void run();

        void quit();

        bool processEvents();

        double totalTime() const;

    private:
        float m_lastDeltaTime; // in seconds
        double m_timeSinceStart; // in seconds

        uint16_t m_width;
        uint16_t m_height;

        InputManager m_input;

        bool m_quit; // should we exit
    };

    void run();

    App* app();
}