#pragma once

#include "input.h"

namespace visef
{
    class InputManager
    {
    public:
        InputManager();

        void update();

        InputDevice& keyboard();
    private:
        InputDevice m_keyboard;
    };
}