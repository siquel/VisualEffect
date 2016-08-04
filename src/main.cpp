#include <stdio.h>

#include <bgfx/bgfxplatform.h>
#include <bx/thread.h>
#include <bx/mutex.h>

#include "event_queue.h"
#include "visef.h"
#include "hid/input.h"
#include "error/error.h"

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720

namespace visef
{
    static bool s_exit = false;

    static uint8_t s_translateKey[256];

    struct MainThreadArgs
    {

    };

    int32_t threadFunc(void* /*data*/)
    {
        visef::run();
        s_exit = true;
        return EXIT_SUCCESS;
    }

    struct Context
    {
        Context() :
            m_hwnd(NULL)
        {
            memset(s_translateKey, 0, sizeof(s_translateKey));
            s_translateKey[VK_CAPITAL] = Key::CapsLock;
            s_translateKey[VK_LCONTROL] = Key::LControl;
            s_translateKey[VK_RCONTROL] = Key::RControl;
            s_translateKey[VK_LSHIFT] = Key::LShift;
            s_translateKey[VK_RSHIFT] = Key::RShift;
            s_translateKey[VK_LMENU] = Key::LAlt;
            s_translateKey[VK_RMENU] = Key::RAlt;
            s_translateKey[VK_LWIN] = Key::LSystem;
            s_translateKey[VK_RWIN] = Key::RSystem;
            s_translateKey[VK_SCROLL] = Key::ScrollLock;
            s_translateKey[VK_MULTIPLY] = Key::Multiply;
            s_translateKey[VK_ADD] = Key::Add;
            s_translateKey[VK_SUBTRACT] = Key::Subtract;
            s_translateKey[VK_NUMLOCK] = Key::Numlock;
            s_translateKey[VK_APPS] = Key::Application;
            s_translateKey[VK_PAUSE] = Key::Pause;
            s_translateKey[VK_DIVIDE] = Key::Divide;
            s_translateKey[VK_ESCAPE] = Key::Escape;
            s_translateKey[VK_RETURN] = Key::Return;
            s_translateKey[VK_TAB] = Key::Tab;
            s_translateKey[VK_BACK] = Key::Backspace;
            s_translateKey[VK_SPACE] = Key::Space;
            s_translateKey[VK_UP] = Key::Up;
            s_translateKey[VK_DOWN] = Key::Down;
            s_translateKey[VK_LEFT] = Key::Left;
            s_translateKey[VK_RIGHT] = Key::Right;
            s_translateKey[VK_INSERT] = Key::Insert;
            s_translateKey[VK_DELETE] = Key::Delete;
            s_translateKey[VK_HOME] = Key::Home;
            s_translateKey[VK_END] = Key::End;
            s_translateKey[VK_PRIOR] = Key::PageUp;
            s_translateKey[VK_NEXT] = Key::PageDown;
            s_translateKey[VK_SNAPSHOT] = Key::Print;
            s_translateKey[VK_OEM_PLUS] = Key::Plus;
            s_translateKey[VK_OEM_MINUS] = Key::Minus;
            s_translateKey[VK_OEM_4] = Key::LeftBracket;
            s_translateKey[VK_OEM_6] = Key::RightBracket;
            s_translateKey[VK_OEM_1] = Key::Semicolon;
            s_translateKey[VK_OEM_7] = Key::Quote;
            s_translateKey[VK_OEM_COMMA] = Key::Comma;
            s_translateKey[VK_OEM_PERIOD] = Key::Period;
            s_translateKey[VK_DECIMAL] = Key::Period;
            s_translateKey[VK_OEM_2] = Key::Slash;
            s_translateKey[VK_OEM_5] = Key::Backslash;
            s_translateKey[VK_OEM_3] = Key::Tilde;
            s_translateKey[VK_F1] = Key::F1;
            s_translateKey[VK_F2] = Key::F2;
            s_translateKey[VK_F3] = Key::F3;
            s_translateKey[VK_F4] = Key::F4;
            s_translateKey[VK_F5] = Key::F5;
            s_translateKey[VK_F6] = Key::F6;
            s_translateKey[VK_F7] = Key::F7;
            s_translateKey[VK_F8] = Key::F8;
            s_translateKey[VK_F9] = Key::F9;
            s_translateKey[VK_F10] = Key::F10;
            s_translateKey[VK_F11] = Key::F11;
            s_translateKey[VK_F12] = Key::F12;
            s_translateKey[VK_F13] = Key::F13;
            s_translateKey[VK_F14] = Key::F14;
            s_translateKey[VK_F15] = Key::F15;
            s_translateKey[VK_F16] = Key::F16;
            s_translateKey[VK_F17] = Key::F17;
            s_translateKey[VK_F18] = Key::F18;
            s_translateKey[VK_F19] = Key::F19;
            s_translateKey[VK_F20] = Key::F20;
            s_translateKey[VK_F21] = Key::F21;
            s_translateKey[VK_F22] = Key::F22;
            s_translateKey[VK_F23] = Key::F23;
            s_translateKey[VK_F24] = Key::F24;
            s_translateKey[VK_NUMPAD0] = Key::NumPad0;
            s_translateKey[VK_NUMPAD1] = Key::NumPad1;
            s_translateKey[VK_NUMPAD2] = Key::NumPad2;
            s_translateKey[VK_NUMPAD3] = Key::NumPad3;
            s_translateKey[VK_NUMPAD4] = Key::NumPad4;
            s_translateKey[VK_NUMPAD5] = Key::NumPad5;
            s_translateKey[VK_NUMPAD6] = Key::NumPad6;
            s_translateKey[VK_NUMPAD7] = Key::NumPad7;
            s_translateKey[VK_NUMPAD8] = Key::NumPad8;
            s_translateKey[VK_NUMPAD9] = Key::NumPad9;
            s_translateKey[uint8_t('0')] = Key::Key0;
            s_translateKey[uint8_t('1')] = Key::Key1;
            s_translateKey[uint8_t('2')] = Key::Key2;
            s_translateKey[uint8_t('3')] = Key::Key3;
            s_translateKey[uint8_t('4')] = Key::Key4;
            s_translateKey[uint8_t('5')] = Key::Key5;
            s_translateKey[uint8_t('6')] = Key::Key6;
            s_translateKey[uint8_t('7')] = Key::Key7;
            s_translateKey[uint8_t('8')] = Key::Key8;
            s_translateKey[uint8_t('9')] = Key::Key9;
            s_translateKey[uint8_t('A')] = Key::KeyA;
            s_translateKey[uint8_t('B')] = Key::KeyB;
            s_translateKey[uint8_t('C')] = Key::KeyC;
            s_translateKey[uint8_t('D')] = Key::KeyD;
            s_translateKey[uint8_t('E')] = Key::KeyE;
            s_translateKey[uint8_t('F')] = Key::KeyF;
            s_translateKey[uint8_t('G')] = Key::KeyG;
            s_translateKey[uint8_t('H')] = Key::KeyH;
            s_translateKey[uint8_t('I')] = Key::KeyI;
            s_translateKey[uint8_t('J')] = Key::KeyJ;
            s_translateKey[uint8_t('K')] = Key::KeyK;
            s_translateKey[uint8_t('L')] = Key::KeyL;
            s_translateKey[uint8_t('M')] = Key::KeyM;
            s_translateKey[uint8_t('N')] = Key::KeyN;
            s_translateKey[uint8_t('O')] = Key::KeyO;
            s_translateKey[uint8_t('P')] = Key::KeyP;
            s_translateKey[uint8_t('Q')] = Key::KeyQ;
            s_translateKey[uint8_t('R')] = Key::KeyR;
            s_translateKey[uint8_t('S')] = Key::KeyS;
            s_translateKey[uint8_t('T')] = Key::KeyT;
            s_translateKey[uint8_t('U')] = Key::KeyU;
            s_translateKey[uint8_t('V')] = Key::KeyV;
            s_translateKey[uint8_t('W')] = Key::KeyW;
            s_translateKey[uint8_t('X')] = Key::KeyX;
            s_translateKey[uint8_t('Y')] = Key::KeyY;
            s_translateKey[uint8_t('Z')] = Key::KeyZ;
        }

        int32_t run(int argc, char** argv)
        {
            SetDllDirectory(".");

            HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

            WNDCLASSEX wnd;
            memset(&wnd, 0, sizeof(WNDCLASSEX));
            wnd.cbSize = sizeof(wnd);
            wnd.style = CS_HREDRAW | CS_VREDRAW;
            wnd.lpfnWndProc = windowProc;
            wnd.hInstance = instance;
            wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
            wnd.lpszClassName = "visef";
            wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
            RegisterClassEx(&wnd);

            m_hwnd = CreateWindowA(
                "visef",
                "Visual effect programming demo",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0, 0,
                WINDOW_DEFAULT_WIDTH,
                WINDOW_DEFAULT_HEIGHT,
                NULL,
                NULL,
                instance,
                0
                );

            bgfx::winSetHwnd(m_hwnd);

            RAWINPUTDEVICE keyboard;
            keyboard.usUsagePage = 0x01;
            keyboard.usUsage = 0x06;
            keyboard.dwFlags = RIDEV_NOLEGACY;        // do not generate legacy messages such as WM_KEYDOWN
            keyboard.hwndTarget = NULL;
            RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));

            BX_UNUSED(argc, argv);
            MainThreadArgs args;

            bx::Thread mainThread;
            mainThread.init(threadFunc, &args);

            MSG msg;
            msg.message = WM_NULL;

            while (!s_exit)
            {
                while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            mainThread.shutdown();
            DestroyWindow(m_hwnd);

            return EXIT_SUCCESS;
        }

        LRESULT process(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
        {
            switch (id)
            {
            case WM_DESTROY:
                break;
            case WM_QUIT:
            case WM_CLOSE:
            {
                s_exit = true;
                m_eventQueue.pushExitEvent();
                return 0;
            }

            case WM_SIZE:
            {
                uint32_t width = LOWORD(lparam);
                uint32_t height = HIWORD(lparam);
                m_eventQueue.pushResolutionEvent(width, height);
            }
            break;

            case WM_INPUT:
            {
                handleRawInput(lparam);
            }
            break;

            default:
                break;
            }

            return DefWindowProc(hwnd, id, wparam, lparam);
        }

        void handleRawInput(LPARAM lparam)
        {
            char buffer[sizeof(RAWINPUT)] = {};
            UINT size = sizeof(RAWINPUT);
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));

            // extract keyboard raw input data
            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
            if (raw->header.dwType == RIM_TYPEKEYBOARD) {
                const RAWKEYBOARD& rawKB = raw->data.keyboard;

                UINT vkey = rawKB.VKey;
                UINT scanCode = rawKB.MakeCode;
                UINT flags = rawKB.Flags;
                // fake key, discard it
                if (vkey == 255) return;

                // the raw input cant figure out left/right vkeys, so we need to do little hax to get them
                // MapVirtualKey will fix the right/left problem, but won't work with Alt and CTRL
                if (vkey == VK_SHIFT) {
                    vkey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
                }
                else if (vkey == VK_NUMLOCK) {
                    // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
                    scanCode = (MapVirtualKey(vkey, MAPVK_VK_TO_VSC) | 0x100);
                }

                const bool isE0 = ((flags & RI_KEY_E0) != 0);
                const bool isE1 = ((flags & RI_KEY_E1) != 0);

                // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
                // however, MapVirtualKey is unable to map VK_PAUSE, hence we map that by hand.
                if (vkey == VK_PAUSE)
                    scanCode = 0x45;
                else
                    scanCode = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);

                // decipher the rest, CTRL, alt and so on..
                switch (vkey)
                {
                case VK_CONTROL: vkey = (isE0) ? VK_RCONTROL : VK_LCONTROL; break;
                case VK_MENU: vkey = (isE0) ? VK_RMENU : VK_LMENU; break;
                    /* numpad enter
                    case VK_RETURN:
                    vkey = (isE0) ? NUMPAD_ENTER
                    */
                    // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
                    // corresponding keys on the NUMPAD will not.
                case VK_INSERT: vkey = (!isE0) ? VK_NUMPAD0 : VK_INSERT; break;
                    // numpad decimal
                case VK_DELETE: vkey = (!isE0) ? VK_DECIMAL : VK_DELETE; break;
                case VK_HOME: vkey = (!isE0) ? VK_NUMPAD7 : VK_HOME; break;
                case VK_END: vkey = (!isE0) ? VK_NUMPAD1 : VK_END; break;
                case VK_PRIOR: vkey = (!isE0) ? VK_NUMPAD9 : VK_PRIOR; break;
                case VK_NEXT: vkey = (!isE0) ? VK_NUMPAD3 : VK_NEXT; break;
                    // the standard arrow keys will always have their e0 bit set, but the
                    // corresponding keys on the NUMPAD will not.
                case VK_LEFT: vkey = (!isE0) ? VK_NUMPAD4 : VK_LEFT; break;
                case VK_RIGHT: vkey = (!isE0) ? VK_NUMPAD6 : VK_RIGHT; break;
                case VK_UP: vkey = (!isE0) ? VK_NUMPAD8 : VK_UP; break;
                case VK_DOWN: vkey = (!isE0) ? VK_NUMPAD2 : VK_DOWN; break;
                case VK_CLEAR: vkey = (!isE0) ? VK_NUMPAD5 : VK_CLEAR; break;
                }

                bool isDown = !((flags & RI_KEY_BREAK) != 0);

                // unknown keys... 
                if (s_translateKey[vkey] == 0 && vkey != uint8_t('A'))
                {
                    VE_ASSERT(0, "Unknown key code = %d", vkey);
                }

                m_eventQueue.pushButtonEvent(
                    InputDeviceType::Keyboard,
                    s_translateKey[vkey],
                    isDown
                    );
            }
        }


        static LRESULT CALLBACK windowProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

        HWND m_hwnd;
        EventQueue m_eventQueue;
    };

    static Context s_ctx;

    LRESULT CALLBACK Context::windowProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
    {
        return s_ctx.process(hwnd, id, wparam, lparam);
    }

    // consumer only
    bool nextEvent(Event& ev)
    {
        return s_ctx.m_eventQueue.pop(ev);
    }
}

int main(int argc, char** argv)
{
    using namespace visef;
    return s_ctx.run(argc, argv);
}