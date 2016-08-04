#pragma once
#include "input.h"
#include <bx/bx.h>
#include "error/error.h"

namespace visef
{
    static const char* s_scancodeNames[] = {
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "Keypad 0",
        "Keypad 1",
        "Keypad 2",
        "Keypad 3",
        "Keypad 4",
        "Keypad 5",
        "Keypad 6",
        "Keypad 7",
        "Keypad 8",
        "Keypad 9",
        "Escape",
        "Left Control",
        "Left Shift",
        "Left Alt",
        "Left System",
        "Right Control",
        "Right Shift",
        "Right Alt",
        "Right System",
        "Menu",
        "Return",
        "Tab",
        "Backspace",
        "Space",
        "Up",
        "Down",
        "Left",
        "Right",
        "Insert",
        "Delete",
        "Home",
        "End",
        "PageUp",
        "PageDown",
        "Print",
        "Plus",
        "Minus",
        "LeftBracket",
        "RightBracket",
        "Semicolon",
        "Quote",
        "Comma",
        "Period",
        "Slash",
        "Backslash",
        "Tilde",
        "CapsLock",
        "ScrollLock",
        "Multiply",
        "Divide",
        "Pause"
    };

    static_assert(BX_COUNTOF(s_scancodeNames) == Key::Count, "Invalid amount of scan codes");

    const char* getKeyName(uint16_t key)
    {
        VE_ASSERT(key < Key::Count, "Key out of bounds");
        return s_scancodeNames[key];
    }

    InputDevice::InputDevice(uint8_t numKeys) :
        m_numButtons(numKeys)
    {
        VE_ASSERT(numKeys < MaxKeys, "Too many keys");
        memset(m_state, 0, sizeof(m_state));
        memset(m_lastState, 0, sizeof(m_lastState));
    }

    InputDevice::~InputDevice() {}

    void InputDevice::update()
    {
        memcpy(m_lastState, m_state, sizeof(uint8_t) * m_numButtons);
    }

    bool InputDevice::isPressed(uint8_t key) const
    {
        VE_ASSERT(key < m_numButtons, "Key out of bounds");
        // ~0 = 1, ~1 = 0
        // if m_lastState == 0 && m_state == 1
        return (~m_lastState[key] & m_state[key]) != 0;
    }

    bool InputDevice::isReleased(uint8_t key) const
    {
        VE_ASSERT(key < m_numButtons, "Key out of bounds");
        // ~0 = 1, ~1 = 0
        // if m_lastState == 1 && m_state == 0
        return (m_lastState[key] & ~m_state[key]) != 0;
    }

    void InputDevice::setKeyState(uint8_t key, bool state)
    {
        VE_ASSERT(key < m_numButtons, "Key out of bounds");
        m_state[key] = state;
    }
}