// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WINDOW_MANAGER_HPP
#define KIRANA_CORE_WINDOW_MANAGER_HPP

#include <iostream>
#include <memory>

#include "no_copy.hpp"
#include "window.hpp"

namespace kirana::core
{
class WindowManager : NoCopy
{
public:
    /**
     * Any window creation / management is performed through the WindowManager
     * object. It keeps track of all the windows and polls window events.
     */
    WindowManager() = default;
    ~WindowManager() = default;

    WindowManager(const WindowManager &manager) = delete;
    WindowManager &operator=(const WindowManager &manager) = delete;

    /**
     * Calls native window initialization functions, before we can proceed with
     * window creation.
     * @return true if successful.
     */
    bool init();
    /**
     * Poll window events such as creation, destruction, moving or resizing. Needs
     * to be called every frame.
     * @return true if successfully polled events.
     */
    bool pollEvents();
    /**
     * Quits and de-initializes native window management.
     */
    void quit();

    /// Returns true if at least one window is visible on the screen.
    [[nodiscard]] bool isAnyWindowActive() const
    {
        if (m_windows.empty())
        {
            return false;
        }
        for (const auto &[_, window] : m_windows)
        {
            if (!window->isClosed())
            {
                return true;
            }
        }
        return false;
    }

    /// Returns the Window object with the given handle.
    [[nodiscard]] const Window &getWindow(const Handle<Window> &handle) const
    {
        return *m_windows.at(handle).get();
    }

    /// Returns the Window object with the given handle.
    [[nodiscard]] Window &getWindow(const Handle<Window> &handle)
    {
        return *m_windows.at(handle).get();
    }

    /// Returns the current window in focus.
    [[nodiscard]] const Window &getFocusedWindow() const
    {
        return getWindow(m_focusedWindow);
    }

    /// Returns the current window in focus.
    [[nodiscard]] Window &getFocusedWindow()
    {
        return getWindow(m_focusedWindow);
    }

    /**
     * Creates a window with the given properties. The window is only created. To
     * show the window on screen, use the showWindow function.
     * @param name Name of the window.
     * @param size Initial size of the window.
     * @param position Initial position of the window on the screen. (0,0) will be the top-left corner of the screen.
     * @param parent Handle of the parent window, if any.
     * @return Handle to the created window. Handle will be -1 if it fails to
     * create the window.
     */
    Handle<Window> createWindow(const std::string &name, WindowSize size, WindowPosition position = DEFAULT_WINDOW_POS,
                                Handle<Window> parent = {});

    /// Displays the window with given handle on the screen.
    void showWindow(const Handle<Window> &handle) const
    {
        if (m_windows.find(handle) == m_windows.end())
        {
            return;
        }
        getWindow(handle).show();
    }

    /// Closes the window with the given handle.
    void closeWindow(const Handle<Window> &handle)
    {
        if (m_windows.find(handle) == m_windows.end())
        {
            return;
        }
        getWindow(handle).close();
        // TODO: Add a proper way to clean-up windows handles and event listeners.
    }

private:
    uint32_t m_window_count = 0;
    std::unordered_map<Handle<Window>, std::unique_ptr<Window>> m_windows = {};
    Handle<Window> m_focusedWindow = {};

    void onWindowEvent(const Handle<Window> &handle, WindowEventType type, const WindowEventData &data);
};
}

#endif  // KIRANA_CORE_WINDOW_MANAGER_HPP