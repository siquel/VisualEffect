#include <stdio.h>

#include <bgfx/bgfxplatform.h>
#include <bx/thread.h>
#include <bx/mutex.h>

#include "event_queue.h"
#include "visef.h"

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720

namespace visef
{
    static bool s_exit = false;

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

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {

            }
            break;

            default:
                break;
            }

            return DefWindowProc(hwnd, id, wparam, lparam);
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