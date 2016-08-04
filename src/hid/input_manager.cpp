#include "input_manager.h"

namespace visef
{
    InputManager::InputManager() :
        m_keyboard(Key::Count)
    {

    }

    void InputManager::update()
    {
        m_keyboard.update();
    }

    InputDevice& InputManager::keyboard()
    {
        return m_keyboard;
    }
}