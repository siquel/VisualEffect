#pragma once
#include <stdint.h>

namespace visef
{
    struct InputDeviceType
    {
        enum Enum
        {
            Keyboard,
            Mouse,
            Gamepad,

            Count
        };
    };

    struct Key 
    {
        enum Enum {
            KeyA = 0,        ///< The A key
            KeyB,            ///< The B key
            KeyC,            ///< The C key
            KeyD,            ///< The D key
            KeyE,            ///< The E key
            KeyF,            ///< The F key
            KeyG,            ///< The G key
            KeyH,            ///< The H key
            KeyI,            ///< The I key
            KeyJ,            ///< The J key
            KeyK,            ///< The K key
            KeyL,            ///< The L key
            KeyM,            ///< The M key
            KeyN,            ///< The N key
            KeyO,            ///< The O key
            KeyP,            ///< The P key
            KeyQ,            ///< The Q key
            KeyR,            ///< The R key
            KeyS,            ///< The S key
            KeyT,            ///< The T key
            KeyU,            ///< The U key
            KeyV,            ///< The V key
            KeyW,            ///< The W key
            KeyX,            ///< The X key
            KeyY,            ///< The Y key
            KeyZ,            ///< The Z key
            Key0,           ///< The 0 key
            Key1,           ///< The 1 key
            Key2,           ///< The 2 key
            Key3,           ///< The 3 key
            Key4,           ///< The 4 key
            Key5,           ///< The 5 key
            Key6,           ///< The 6 key
            Key7,           ///< The 7 key
            Key8,           ///< The 8 key
            Key9,           ///< The 9 key
            F1,           ///< The F1 key
            F2,           ///< The F2 key
            F3,           ///< The F3 key
            F4,           ///< The F4 key
            F5,           ///< The F5 key
            F6,           ///< The F6 key
            F7,           ///< The F7 key
            F8,           ///< The F8 key
            F9,           ///< The F9 key
            F10,          ///< The F10 key
            F11,          ///< The F11 key
            F12,          ///< The F12 key
            F13,          ///< The F13 key
            F14,          ///< The F14 key
            F15,          ///< The F15 key
            F16,          ///< The F16 key
            F17,          ///< The F17 key
            F18,          ///< The F18 key
            F19,          ///< The F19 key
            F20,          ///< The F20 key
            F21,          ///< The F21 key
            F22,          ///< The F22 key
            F23,          ///< The F23 key
            F24,          ///< The F24 key
            NumPad0,      ///< The numpad 0 key
            NumPad1,      ///< The numpad 1 key
            NumPad2,      ///< The numpad 2 key
            NumPad3,      ///< The numpad 3 key
            NumPad4,      ///< The numpad 4 key
            NumPad5,      ///< The numpad 5 key
            NumPad6,      ///< The numpad 6 key
            NumPad7,      ///< The numpad 7 key
            NumPad8,      ///< The numpad 8 key
            NumPad9,      ///< The numpad 9 key
            Escape,       ///< The Escape key
            LControl,     ///< The left Control key
            LShift,       ///< The left Shift key
            LAlt,         ///< The left Alt key
            LSystem,      ///< The left OS specific key
            RControl,     ///< The right Control key
            RShift,       ///< The right Shift key
            RAlt,         ///< The right Alt key
            RSystem,      ///< The right OS specific key
            Menu,         ///< The Menu key
            Return,       ///< The Enter key
            Tab,          ///< The Tab key
            Backspace,    ///< The backspace key
            Space,        ///< The Space key
            Up,
            Down,
            Left,
            Right,
            Insert,
            Delete,
            Home,
            End,
            PageUp,
            PageDown,
            Print,
            Plus,
            Minus,
            LeftBracket,
            RightBracket,
            Semicolon,
            Quote,
            Comma,
            Period,
            Slash,
            Backslash,
            Tilde,
            CapsLock,
            ScrollLock,
            Multiply, ///< The numpad multiply key
            Divide,   ///< The numpad divide key
            Pause,
            Add,      ///< The numpad add key
            Subtract, ///< The numpad subtract key
            Numlock,
            Application, ///< The applications key
            Count
        };
    };

    const char* getKeyName(uint8_t key);

    struct InputDevice
    {
        const static uint32_t MaxKeys = 256;

        InputDevice(uint8_t numKeys);

        ~InputDevice();

        bool isPressed(uint8_t key) const;

        bool isReleased(uint8_t key) const;

        void setKeyState(uint8_t key, bool state);

        void update();

        uint8_t m_numButtons;
        uint8_t m_lastState[MaxKeys];
        uint8_t m_state[MaxKeys];
    };
}