// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "window_manager.hpp"

#include "logger.hpp"

#if PLATFORM == WINDOWS
#include "win32_window.hpp"
#endif

namespace kirana::core
{
void WindowManager::onWindowEvent(const Handle<Window> &handle, const WindowEventType type, const WindowEventData &data)
{
    switch (type)
    {
    case WindowEventType::FOCUSED:
        if (data.focused)
            m_focusedWindow = handle;
        break;
    case WindowEventType::CLOSING:
        closeWindow(handle);
        break;
    default:
        break;
    }
}

bool WindowManager::init()
{
#if PLATFORM == WINDOWS
    return initializeWin32();
#else
    // TODO: Add other platform window init event code.
#endif
    return true;
}

bool WindowManager::pollEvents()
{
#if PLATFORM == WINDOWS
    return pollWin32Events();
#else
    // TODO: Add other platform window poll event code.
#endif
    return true;
}

void WindowManager::quit()
{
#if PLATFORM == WINDOWS
    quitWin32();
#else
    // TODO: Add other platform window quit code.
#endif
}

Handle<Window> WindowManager::createWindow(const std::string &name, WindowSize size, WindowPosition position,
                                           const Handle<Window> parent)
{
    const Window *p_parent = parent.isValid() ? m_windows.at(parent).get() : nullptr;

    auto handle = Handle<Window>(m_window_count++);
    m_windows.insert({handle, std::make_unique<Window>(name, size, position, p_parent)
    });
    const auto &p_window = m_windows.at(handle);
    if (p_window->create())
    {
        p_window->addOnWindowEventListener([&, handle](const WindowEventType type,
                                               const WindowEventData &data) {
            onWindowEvent(handle, type, data);
        });
    }
    else
    {
        Logger::get().error("Failed to create window \"" + name + "\"");
        m_windows.erase(handle);
        return Handle<Window>();
    }
    return handle;
}
} // namespace kirana