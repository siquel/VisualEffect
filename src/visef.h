#pragma once


namespace visef
{
    struct App
    {
        App();

        ~App();

        void run();

        void quit();

        bool processEvents();

        float m_lastDeltaTime; // in seconds
        double m_timeSinceStart; // in seconds

        bool m_quit; // should we exit
    };

    void run();

    App* app();
}