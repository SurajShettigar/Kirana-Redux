// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "window_manager.hpp"

// Select the platform implementation at compile time
#if PLATFORM == WINDOWS
#include "win32_window.hpp"
using PlatformWindow = kirana::core::Win32Window;
#elif PLATFORM == MAC
// #include "cocoa_window.hpp"
// using PlatformWindow = kirana::core::CocoaWindow;
#error "macOS window implementation not yet available"
#elif PLATFORM == LINUX
// #include "x11_window.hpp"
// using PlatformWindow = kirana::core::X11Window;
#error "Linux (X11/Wayland) window implementation not yet available"
#else
#error "Unsupported platform"
#endif

namespace kirana::core
{

IWindow *WindowManager::getWindow(const WindowHandle handle) const
{
    return m_windows.contains(handle) ? m_windows.at(handle).get() : nullptr;
}


WindowHandle WindowManager::createWindow(const WindowDesc &desc)
{
    const WindowHandle handle = m_next_handle++;

    m_windows.emplace(handle, std::make_unique<PlatformWindow>());
    if (!m_windows.at(handle)->init(desc, handle, m_event_manager))
    {
        m_windows.erase(handle);
        return WindowHandle{};
    }
    return handle;
}

void WindowManager::closeWindow(const WindowHandle handle) const
{
    if (handle.isValid() && m_windows.contains(handle))
    {
        m_windows.at(handle)->destroy();
    }
}

void WindowManager::closeAll() const
{
    for (auto &window : m_windows | std::views::values)
        window->destroy();
}

void WindowManager::pollAll()
{
    for (const auto &window : m_windows | std::views::values)
        window->pollEvents();
    pruneClosedWindows();
}

void WindowManager::poll(const WindowHandle handle) const
{
    if (IWindow *win = getWindow(handle))
        win->pollEvents();
}

void WindowManager::pruneClosedWindows()
{
    // If there are destroyed window's handles in the list, it means we have already closed the window and just
    // need to remove its reference from the map.
    for (const auto handle : m_destroyed_windows)
    {
        m_windows.erase(handle);
    }
    m_destroyed_windows.clear();
}

bool WindowManager::hasOpenWindows() const
{
    return !m_windows.empty();
}

bool WindowManager::isValid(const WindowHandle handle) const
{
    return m_windows.contains(handle);
}

bool WindowManager::isOpen(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w && w->isOpen();
}

bool WindowManager::isFocused(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w && w->isFocused();
}

bool WindowManager::isMinimized(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w && w->isMinimized();
}

bool WindowManager::isVisible(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w && w->isVisible();
}

bool WindowManager::isResizable(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w && w->isResizable();
}

WindowSize WindowManager::getSize(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w ? w->getSize() : WindowSize{};
}

WindowPosition WindowManager::getPosition(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w ? w->getPosition() : WindowPosition{-1, -1};
}

std::string WindowManager::getTitle(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w ? w->getTitle() : "";
}

void WindowManager::setTitle(const WindowHandle handle, const std::string &title) const
{
    if (IWindow *w = getWindow(handle))
        w->setTitle(title);
}

void WindowManager::resize(const WindowHandle handle, const WindowSize &size) const
{
    if (IWindow *w = getWindow(handle))
        w->resize(size);
}

void WindowManager::move(const WindowHandle handle, const WindowPosition &position) const
{
    if (IWindow *w = getWindow(handle))
        w->move(position);
}

void WindowManager::show(const WindowHandle handle) const
{
    if (IWindow *w = getWindow(handle))
        w->show();
}

void WindowManager::hide(const WindowHandle handle) const
{
    if (IWindow *w = getWindow(handle))
        w->hide();
}

void *WindowManager::getNativeWindowHandle(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w ? w->getNativeWindowHandle() : nullptr;
}

void *WindowManager::getNativeDisplayHandle(const WindowHandle handle) const
{
    const IWindow *w = getWindow(handle);
    return w ? w->getNativeDisplayHandle() : nullptr;
}

void WindowManager::onEvent(const Event &event)
{
    if (event.getType() == WindowEvent::getStaticType())
    {
        const auto &win_event = dynamic_cast<const WindowEvent &>(event);
        if (win_event.status == WindowStatus::CLOSING)
        {
            closeWindow(win_event.handle);
        }
        if (win_event.status == WindowStatus::DESTROYED)
        {
            // We just add the window's handle to the list. The IWindow object will be destroyed when polling for
            // window events in the next frame. This ensures the IWindow object references remains valid during its
            // "destroy" event processing and is only destroyed in the next frame.
            m_destroyed_windows.push_back(win_event.handle);
        }
    }
}
} // namespace kirana::core