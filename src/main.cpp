#include <stdio.h>

#include <bgfx/bgfxplatform.h>
#include <bx/thread.h>
#include <bx/mutex.h>

#include "event_queue.h"
#include "visef.h"
#include "hid/input.h"

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720

namespace visef
{
    static bool s_exit = false;

    Key::Enum translateKey(UINT vkey) 
    {
        switch (vkey)
        {

        case VK_ESCAPE: return Key::Escape;
        case VK_LCONTROL: return Key::LControl;
        case VK_LSHIFT: return Key::LShift;
        case VK_LMENU: return Key::LAlt;
        case VK_LWIN: return Key::LSystem;

        case VK_RCONTROL: return Key::RControl;
        case VK_RSHIFT: return Key::RShift;
        case VK_RMENU: return Key::RAlt;
        case VK_RWIN: return Key::RSystem;
            // Digits
        case 0x30: return Key::D0;
        case 0x31: return Key::D1;
        case 0x32: return Key::D2;
        case 0x33: return Key::D3;
        case 0x34: return Key::D4;
        case 0x35: return Key::D5;
        case 0x36: return Key::D6;
        case 0x37: return Key::D7;
        case 0x38: return Key::D8;
        case 0x39: return Key::D9;

            // Letters
        case 0x41: return Key::A;
        case 0x42: return Key::B;
        case 0x43: return Key::C;
        case 0x44: return Key::D;
        case 0x45: return Key::E;
        case 0x46: return Key::F;
        case 0x47: return Key::G;
        case 0x48: return Key::H;
        case 0x49: return Key::I;

        case 0x4A: return Key::J;
        case 0x4B: return Key::K;
        case 0x4C: return Key::L;
        case 0x4D: return Key::M;
        case 0x4E: return Key::N;
        case 0x4F: return Key::O;

        case 0x50: return Key::P;
        case 0x51: return Key::Q;
        case 0x52: return Key::R;
        case 0x53: return Key::S;
        case 0x54: return Key::T;
        case 0x55: return Key::U;
        case 0x56: return Key::V;
        case 0x57: return Key::W;
        case 0x58: return Key::X;
        case 0x59: return Key::Y;

        case 0x5A: return Key::Z;

        case VK_NUMPAD0: return Key::Numpad0;
        case VK_NUMPAD1: return Key::Numpad1;
        case VK_NUMPAD2: return Key::Numpad2;
        case VK_NUMPAD3: return Key::Numpad3;
        case VK_NUMPAD4: return Key::Numpad4;
        case VK_NUMPAD5: return Key::Numpad5;
        case VK_NUMPAD6: return Key::Numpad6;
        case VK_NUMPAD7: return Key::Numpad7;
        case VK_NUMPAD8: return Key::Numpad8;
        case VK_NUMPAD9: return Key::Numpad9;

        case VK_F1: return Key::F1;
        case VK_F2: return Key::F2;
        case VK_F3: return Key::F3;
        case VK_F4: return Key::F4;
        case VK_F5: return Key::F5;
        case VK_F6: return Key::F6;
        case VK_F7: return Key::F7;
        case VK_F8: return Key::F8;
        case VK_F9: return Key::F9;
        case VK_F10: return Key::F10;
        case VK_F11: return Key::F11;
        case VK_F12: return Key::F12;
        case VK_F13: return Key::F13;
        case VK_F14: return Key::F14;
        case VK_F15: return Key::F15;
        case VK_F16: return Key::F16;
        case VK_F17: return Key::F17;
        case VK_F18: return Key::F18;
        case VK_F19: return Key::F19;
        case VK_F20: return Key::F20;
        case VK_F21: return Key::F21;
        case VK_F22: return Key::F22;
        case VK_F23: return Key::F23;
        case VK_F24: return Key::F24;

            // TODO add more keys
        default: return Key::Count;
        }
    }

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
                
                m_eventQueue.pushButtonEvent(
                    InputDeviceType::Keyboard,
                    translateKey(vkey),
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