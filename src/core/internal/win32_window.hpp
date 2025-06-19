// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WIN32_WINDOW_HPP
#define KIRANA_CORE_WIN32_WINDOW_HPP

#include <windows.h>

#include "window_constants.hpp"

namespace kirana::core
{

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowEventCallback *callback = nullptr;
    if (uMsg == WM_NCCREATE)
    {
        auto p_create = reinterpret_cast<CREATESTRUCT *>(lParam);
        callback =
            reinterpret_cast<WindowEventCallback *>(p_create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(callback));
        return TRUE;
    }
    else
    {
        callback = reinterpret_cast<WindowEventCallback *>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }

    WindowEventType type;
    WindowEventData data = {};
    switch (uMsg)
    {
    case WM_ACTIVATEAPP:
        type = WindowEventType::FOCUSED;
        data.focused = static_cast<bool>(wParam);
        break;
    case WM_CLOSE:
        type = WindowEventType::CLOSING;
        break;
    case WM_CREATE:
        type = WindowEventType::CREATED;
        break;
    case WM_DESTROY:
        type = WindowEventType::CLOSED;
        break;
    case WM_ENABLE:
        type = WindowEventType::ENABLED;
        data.enabled = static_cast<bool>(wParam);
        break;
    case WM_ENTERSIZEMOVE:
        type = WindowEventType::MOVING_RESIZING_START;
        break;
    case WM_EXITSIZEMOVE:
        type = WindowEventType::MOVING_RESIZING_END;
        break;
    case WM_MOVE: {
        type = WindowEventType::MOVING;
        const int x = LOWORD(lParam);
        const int y = HIWORD(lParam);
        data.position =
            WindowPosition{static_cast<uint16_t>(x), static_cast<uint16_t>(y)};
    }
    break;
    case WM_SIZE: {
        type = WindowEventType::RESIZING;
        const int width = LOWORD(lParam);
        const int height = HIWORD(lParam);
        data.size = WindowSize{static_cast<uint16_t>(width),
                               static_cast<uint16_t>(height)};
    }
    break;
    default:
        type = WindowEventType::UNKNOWN;
    }
    if (callback != nullptr && type != WindowEventType::UNKNOWN)
    {
        (*callback)(type, data);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

inline bool initializeWin32()
{
    HINSTANCE instance = GetModuleHandle(nullptr);
    WNDCLASS w_class = {};
    w_class.hCursor = LoadCursor(instance, IDC_ARROW);
    w_class.hInstance = instance;
    w_class.lpfnWndProc = WindowProc;
    w_class.lpszClassName = WINDOW_EVENT_CLASS_NAME.c_str();

    return RegisterClass(&w_class) != 0;
}

inline HWND createWin32Window(HWND parent, const std::string &name, WindowSize size, WindowPosition position,
                              WindowEventCallback *eventCallback)
{
    DWORD style = WS_OVERLAPPEDWINDOW;

    HWND handle = CreateWindowEx(
        0, WINDOW_EVENT_CLASS_NAME.c_str(), name.c_str(), style,
        position.x, position.y, size.width, size.height,
        parent, nullptr, GetModuleHandle(nullptr), eventCallback);

    return handle;
}

inline void showWin32Window(HWND handle)
{
    ShowWindow(handle, SW_NORMAL);
}

inline void focusWin32Window(HWND handle)
{
    SetFocus(handle);
}

inline void closeWin32Window(HWND handle)
{
    DestroyWindow(handle);
}

inline bool pollWin32Events()
{
    MSG msg = {};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
    {
        //  if (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
    return false;
}

inline void quitWin32()
{
    PostQuitMessage(0);
}

} // namespace kirana

#endif  // KIRANA_CORE_WIN32_WINDOW_HPP