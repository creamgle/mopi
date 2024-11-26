#include "mopi/mopi.h"

#ifdef MOPI_PLATFORM_WIN32
#include "internal.h"
#include "containers/mopi_vector.h"
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>

#define _WINDOW_CLASS_NAME  L"Mopi Window Class"

static struct mopi_State {
    HINSTANCE* hInstance;
    WNDCLASSEX wc;
    mopi_Window** registered_windows;
} *_state;

struct _WindowInternal {
    HWND hwnd;
    MOPI_BOOL close_requested;
};

WCHAR* CharPtrToWCHAR(const char* str) {
    int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    WCHAR* wideStr = (WCHAR*)malloc(length * sizeof(WCHAR));
    if (wideStr) {
        MultiByteToWideChar(CP_UTF8, 0, str, -1, wideStr, length);
    }
    return wideStr;
}

static mopi_Window* _GetWindowStructFromHWND(HWND hwnd);
LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int mopi_Init(void)
{
    mopiInternal_SendMessage(MOPI_MESSAGE_TRACE, MOPI_MSG_INITIALISE_BEGIN);

    _state = malloc(sizeof(struct mopi_State));
    _state->hInstance = GetModuleHandle(0);
    if (!_state->hInstance)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_FATAL, MOPI_MSG_WIN32_INITIALISE_HINSTANCE_GET_FAIL);
        return MOPI_FAIL;
    }

    _state->wc.cbSize = sizeof(WNDCLASSEX);
    _state->wc.style = 0;
    _state->wc.lpfnWndProc = _WindowProc;
    _state->wc.cbClsExtra = 0;
    _state->wc.cbWndExtra = 0;
    _state->wc.hInstance = _state->hInstance;
    _state->wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    _state->wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    _state->wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    _state->wc.lpszMenuName = NULL;
    _state->wc.lpszClassName = _WINDOW_CLASS_NAME;
    _state->wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&_state->wc))
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_FATAL, MOPI_MSG_WIN32_INITIALISE_REGISTER_WINDOW_CLASS_FAIL);
        return MOPI_FAIL;
    }

    _state->registered_windows = mopi_VectorCreate(mopi_Window*);

    return MOPI_PASS;
}

void mopi_Destroy(void)
{
    mopiInternal_SendMessage(MOPI_MESSAGE_TRACE, MOPI_MSG_DESTROY_BEGIN);
    mopi_VectorDestroy(_state->registered_windows);
}

void mopi_SetMessageCallback(void (*cbfun_message)(int level, const char* message))
{
    internal_callbacks.cbfun_message = cbfun_message;
}

void mopiInternal_SendMessage(int level, const char* message)
{
    if (!internal_callbacks.cbfun_message)
        return;

    internal_callbacks.cbfun_message(level, message);
}

mopi_Window* mopi_CreateWindow(int width, int height, const char* title)
{
    mopi_Window* window = malloc(sizeof(mopi_Window));
    if (!window)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_FATAL, "Failed to allocate memory for struct mopi_Window!");
        return MOPI_FAIL;
    }

    window->internal = malloc(sizeof(struct _WindowInternal));
    if (!window->internal)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_FATAL, "Failed to allocate memory for struct _WindowInternal!");
        return MOPI_FAIL;
    }

    DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
    DWORD style = WS_OVERLAPPEDWINDOW;

    window->internal->hwnd = CreateWindowEx(
        exStyle,
        _WINDOW_CLASS_NAME,
        CharPtrToWCHAR(title),
        style,

        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,

        NULL,
        NULL,
        _state->hInstance,
        NULL
    );

    if (!window->internal->hwnd)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_FATAL, "Failed to create hwnw from CreateWindowEx");
        return MOPI_FAIL;
    }

    window->internal->close_requested = MOPI_FAIL;
    
    ShowWindow(window->internal->hwnd, SW_SHOW);
    
    mopi_VectorPush(_state->registered_windows, window);

    return window;
}

void mopi_DestroyWindow(mopi_Window* window)
{
    DestroyWindow(window->internal->hwnd);
}

MOPI_BOOL mopi_WindowShouldClose(mopi_Window* window)
{
    return window->internal->close_requested;
}

void mopi_PollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

MOPI_API void mopi_SetWindowSize(mopi_Window* window, int width, int height)
{
    if (!window)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Attempting to change the size of a NULL window!");
        return;
    }
    
    UINT resize_flags = SWP_NOMOVE;
    SetWindowPos(window->internal->hwnd, NULL, -1, -1, width, height, resize_flags);
}

void mopi_WindowMaximize(mopi_Window* window)
{
    if (!window)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Attempting to maximize a NULL window!");
        return;
    }

    PostMessage(window->internal->hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void mopi_WindowMinimize(mopi_Window* window)
{
    if (!window)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Attempting to minimize a NULL window!");
        return;
    }

    PostMessage(window->internal->hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void mopi_SetWindowResizeCallback(mopi_Window* window, void (*cbfun_resize)(mopi_Window* window, int width, int height))
{
    if (!window->internal->hwnd)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, MOPI_MSG_ATTACH_CALLBACK_RESIZE_FAIL_INVALID_WINDOW);
        return;
    }

    window->callbacks.cbfun_resize = cbfun_resize;
}

void mopi_GetCursorPosition(mopi_Window* window, int* out_x, int* out_y)
{
    POINT point;
    if (!GetCursorPos(&point))
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Failed to get the cursor position of the specified window!");
        return;
    }

    if (!ScreenToClient(window->internal->hwnd, &point))
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Failed to convert screen cursor pos to window based!");
        return;
    }

    *out_x = point.x;
    *out_y = point.y;
}

void mopi_SetCursorPositionCallback(mopi_Window* window, void (*cbfun_cursorpos)(mopi_Window* window, int x, int y))
{
    if (!window->internal->hwnd)
    {
        mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, "Failed to attach a [CursorPositionChange] callback to the window because its invalid!");
        return;
    }

    window->callbacks.cbfun_cursorpos = cbfun_cursorpos;
}

LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mopi_Window* window = _GetWindowStructFromHWND(hwnd);

    switch (uMsg)
    {
        case WM_CLOSE:
        {   
            if (window)
                window->internal->close_requested = MOPI_OKAY;
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
        {
            int width = (int)LOWORD(lParam);
            int height = (int)HIWORD(lParam);

            if (!window)
                return 0;

            if (window->callbacks.cbfun_resize)
                window->callbacks.cbfun_resize(window, width, height);

            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (!window)
                return 0;

            if (window->callbacks.cbfun_cursorpos)
            {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);

                window->callbacks.cbfun_cursorpos(window, x, y);
            }
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static mopi_Window* _GetWindowStructFromHWND(HWND hwnd)
{
    for (unsigned int i = 0; i < mopi_VectorLength(_state->registered_windows); ++i) {
        mopi_Window* window = _state->registered_windows[i];
        if (!window)
           continue;

        if (window->internal->hwnd == hwnd)
            return _state->registered_windows[i];
            
    }

    return NULL;
}

#endif