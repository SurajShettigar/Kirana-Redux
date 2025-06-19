// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "window.hpp"

#if PLATFORM == WINDOWS
#include "win32_window.hpp"

/// Creates a new pointer object holding the native window handle.
inline void *nativeHandleToPointer(HWND handle)
{
    if (handle == nullptr)
        return nullptr;
    void *ptr = malloc(sizeof(HWND));
    std::memcpy(ptr, &handle, sizeof(HWND));
    return ptr;
}

/// Creates a new pointer object holding the native window handle.
inline void *nativeAppInstanceToPointer(HINSTANCE handle)
{
    if (handle == nullptr)
        return nullptr;
    void *ptr = malloc(sizeof(HINSTANCE));
    std::memcpy(ptr, &handle, sizeof(HINSTANCE));
    return ptr;
}

/// Gets the native window handle from the given pointer to handle.
inline HWND getNativeHandle(void *ptr)
{
    return *reinterpret_cast<HWND *>(ptr);
}

/// Gets the native window handle from the given pointer to handle.
inline HINSTANCE getNativeAppInstanceHandle(void *ptr)
{
    return *reinterpret_cast<HINSTANCE *>(ptr);
}
#endif

namespace kirana::core
{
void Window::onWindowEvent(WindowEventType type, const WindowEventData &data)
{
    switch (type)
    {
    case WindowEventType::FOCUSED:
        m_focused = data.focused;
        break;
    case WindowEventType::ENABLED:
        m_enabled = data.enabled;
        break;
    case WindowEventType::CLOSED:
        m_closed = true;
        break;
    case WindowEventType::MOVING:
        m_position = data.position;
        break;
    case WindowEventType::RESIZING:
        m_size = data.size;
        break;
    default:
        break;
    }
    for (const auto &l : m_listeners)
        l(type, data);
}

bool Window::create()
{
    if (m_closed)
        return false;
#if PLATFORM == WINDOWS
    HWND native_handle = createWin32Window(
        m_parent == nullptr ? nullptr : getNativeHandle(m_parent->m_native_ptr),
        m_name, m_size, m_position, &m_native_callback);
    m_native_ptr = nativeHandleToPointer(native_handle);
    m_native_app_ptr = nativeAppInstanceToPointer(GetModuleHandle(nullptr));
    return m_native_ptr != nullptr && m_native_app_ptr != nullptr;
#else
    // TODO: Add other platform window creation code.
#endif
}

void Window::show() const
{
    if (m_closed)
        return;
#if PLATFORM == WINDOWS
    showWin32Window(getNativeHandle(m_native_ptr));
#else
    // TODO: Add other platform window show code.
#endif
}

void Window::focus() const
{
    if (m_closed)
        return;
#if PLATFORM == WINDOWS
    focusWin32Window(getNativeHandle(m_native_ptr));
#else
    // TODO: Add other platform window show code.
#endif
}

void Window::close() const
{
    if (m_closed)
        return;
#if PLATFORM == WINDOWS
    closeWin32Window(getNativeHandle(m_native_ptr));
#else
    // TODO: Add other platform window close code.
#endif
}
} // namespace kirana