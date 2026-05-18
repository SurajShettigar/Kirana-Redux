// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "win32_window.hpp"

namespace kirana::core
{
static const std::string WINDOW_CLASS_NAME = "KiranaWin32Window";
static int g_window_class_ref_count = 0;

bool Win32Window::registerWindowClass(HINSTANCE hinstance)
{
    if (g_window_class_ref_count > 0)
    {
        ++g_window_class_ref_count;
        return true;
    }

    WNDCLASS wc{};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(hinstance, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS_NAME.c_str();

    if (!RegisterClass(&wc))
        return false;

    ++g_window_class_ref_count;
    return true;
}

void Win32Window::unregisterWindowClass(HINSTANCE hinstance)
{
    if (--g_window_class_ref_count == 0)
        UnregisterClass(WINDOW_CLASS_NAME.c_str(), hinstance);
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window *self = nullptr;
    if (uMsg == WM_NCCREATE)
    {
        // Store the Win32Window pointer in the HWND user data
        const auto cs = reinterpret_cast<CREATESTRUCT *>(lParam);
        self = static_cast<Win32Window *>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        return TRUE;
    }

    self = reinterpret_cast<Win32Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (self)
        return self->handleMessage(uMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Win32Window::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE: {
        // Signal intent to close — window is NOT destroyed yet.
        // WindowManager observes CLOSING and calls destroy() on WM_DESTROY event.
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::CLOSING});
        return 0;
    }
    case WM_DESTROY: {
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::DESTROYED});
    }

    case WM_ENTERSIZEMOVE: {
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::MOVING_RESIZING_START});
        return 0;
    }

    case WM_EXITSIZEMOVE: {
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::MOVING_RESIZING_END});
        return 0;
    }

    case WM_SIZE: {
        // SIZE_MINIMIZED has zero dimensions — don't treat as a resize.
        if (wParam == SIZE_MINIMIZED)
        {
            m_minimized = true;
            if (m_event_manager)
                m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::MINIMIZED});
        }
        else
        {
            m_minimized = false;
            const uint16_t new_w = LOWORD(lParam);
            const uint16_t new_h = HIWORD(lParam);
            if (new_w != m_desc.size.width || new_h != m_desc.size.height)
            {
                m_desc.size.width = new_w;
                m_desc.size.height = new_h;
                if (m_event_manager)
                    m_event_manager->dispatchEvent(WindowEvent::resizing(m_handle, new_w, new_h));
            }
        }
        return 0;
    }

    case WM_MOVE: {
        m_desc.position.x = LOWORD(lParam);
        m_desc.position.y = HIWORD(lParam);
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent::moving(m_handle, m_desc.position.x, m_desc.position.y));
        return 0;
    }

    case WM_SETFOCUS: {
        m_focused = true;
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent::focused(m_handle, true));
        return 0;
    }

    case WM_KILLFOCUS: {
        m_focused = false;
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent::focused(m_handle, false));
        return 0;
    }
    case WM_ACTIVATEAPP:
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent::focused(m_handle, wParam == TRUE));
        return 0;

    case WM_ENABLE: {
        if (m_event_manager)
            m_event_manager->dispatchEvent(WindowEvent::enabled(m_handle, wParam == TRUE));
        return 0;
    }
    default:
        break;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

bool Win32Window::init(const WindowDesc &desc, const WindowHandle handle, EventManager *event_manager)
{
    if (!IWindow::init(desc, handle, event_manager))
    {
        return false;
    }
    m_hinstance = GetModuleHandle(nullptr);
    if (!registerWindowClass(m_hinstance))
    {
        return false;
    }

    RECT rect{0, 0, m_desc.size.width, m_desc.size.height};
    const DWORD style =
        m_desc.resizable ? (WS_OVERLAPPEDWINDOW) : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
    AdjustWindowRect(&rect, style, FALSE);

    const int size_x = rect.right - rect.left;
    const int size_y = rect.bottom - rect.top;

    const int pos_x = (m_desc.position.x < 0) ? CW_USEDEFAULT : m_desc.position.x;
    const int pos_y = (m_desc.position.y < 0) ? CW_USEDEFAULT : m_desc.position.y;

    m_hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME.c_str(), m_desc.title.c_str(), style, pos_x, pos_y, size_x, size_y,
                            nullptr, nullptr, m_hinstance, this);

    if (!m_hwnd)
    {
        unregisterWindowClass(m_hinstance);
        return false;
    }

    // Read back actual window position
    RECT winRect{};
    GetWindowRect(m_hwnd, &winRect);
    m_desc.position.x = winRect.left;
    m_desc.position.y = winRect.top;

    m_open = true;
    m_focused = (GetForegroundWindow() == m_hwnd);

    if (m_event_manager)
        m_event_manager->dispatchEvent(WindowEvent{m_handle, WindowStatus::CREATED});

    if (desc.visible)
        show();

    DrawMenuBar(m_hwnd);
    UpdateWindow(m_hwnd);

    return true;
}

void Win32Window::destroy()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
        unregisterWindowClass(m_hinstance);
    }
    IWindow::destroy();
}

void Win32Window::pollEvents()
{
    MSG msg = {};
    while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Window::setTitle(const std::string &title)
{
    IWindow::setTitle(title);
    SetWindowText(m_hwnd, m_desc.title.c_str());
}

void Win32Window::resize(const WindowSize &size)
{
    IWindow::resize(size);

    RECT rect{0, 0, m_desc.size.width, m_desc.size.height};
    const auto style = static_cast<DWORD>(GetWindowLong(m_hwnd, GWL_STYLE));
    AdjustWindowRect(&rect, style, FALSE);

    const int size_x = rect.right - rect.left;
    const int size_y = rect.bottom - rect.top;

    SetWindowPos(m_hwnd, nullptr, 0, 0, size_x, size_y, SWP_NOMOVE | SWP_NOZORDER);
}

void Win32Window::move(const WindowPosition &position)
{
    IWindow::move(position);

    const int x = (m_desc.position.x < 0) ? CW_USEDEFAULT : m_desc.position.x;
    const int y = (m_desc.position.y < 0) ? CW_USEDEFAULT : m_desc.position.y;

    SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Read back actual window position
    RECT winRect{};
    GetWindowRect(m_hwnd, &winRect);
    m_desc.position.x = winRect.left;
    m_desc.position.y = winRect.top;
}

void Win32Window::show()
{
    IWindow::show();
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

void Win32Window::hide()
{
    IWindow::hide();
    ShowWindow(m_hwnd, SW_HIDE);
}
} // namespace kirana::core