// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WINDOW_HPP
#define KIRANA_CORE_WINDOW_HPP

#include <string>

#include "no_copy.hpp"
#include "resource_manager.hpp"
#include "window_constants.hpp"

namespace kirana::core
{
class Window : NoCopy
{
public:
    /// Initializes a window with default name, size and position.
    Window() = default;
    /***
     * Initialize a window with the given properties.
     * @param name Name of the window.
     * @param size Initial size of the window.
     * @param position Initial position of the window on the screen. (0,0) will be
     * the top-left corner of the screen.
     * @param parent Pointer to the parent window, if any.
     */
    explicit Window(std::string name, const WindowSize size, const WindowPosition position, const Window *parent)
        : m_parent{parent},
          m_name{std::move(name)},
          m_size{size},
          m_position{position},
          m_native_callback{
              [&](const WindowEventType type, const WindowEventData &data) {
                  onWindowEvent(type, data);
              }
          }
    {
    }

    ~Window()
    {
        if (m_native_ptr != nullptr)
        {
            free(m_native_ptr);
            m_native_ptr = nullptr;
        }
        if (m_native_app_ptr != nullptr)
        {
            free(m_native_app_ptr);
            m_native_app_ptr = nullptr;
        }
    }


    const WindowSize &getSize() const
    {
        return m_size;
    }

    /**
     * Creates the window with the properties initialized by constructor. The
     * window is only created. Use the show function to actually display it on
     * screen.
     * @return true if successful.
     */
    bool create();
    /// Displays the created window on screen.
    void show() const;
    /// Makes the current window active and in focus.
    void focus() const;
    /// Closes / destroys the current window.
    void close() const;

    /**
     * Adds an event listener for window events.
     * @param callback Function to call for window events.
     */
    void addOnWindowEventListener(const WindowEventCallback &callback) const
    {
        return m_listeners.push_back(callback);
    }

    /**
     * Returns the native window pointer based on the current platform. You will need
     * to cast it to the appropriate native handle for the window.
     * @return Pointer to the native window handle.
     */
    [[nodiscard]] const void *getNativeWindowPointer() const
    {
        return m_native_ptr;
    }

    /**
     * Returns the native window pointer based on the current platform. You will need
     * to cast it to the appropriate native handle for the window.
     * @return Pointer to the native window handle.
     */
    [[nodiscard]] void *getNativeWindowPointer()
    {
        return m_native_ptr;
    }

    /**
     * Returns the native window application instance pointer based on the current
     * platform. You will need to cast it to the appropriate native handle.
     * @return Pointer to the native window application instance handle.
     */
    [[nodiscard]] const void *getNativeAppInstancePointer() const
    {
        return m_native_app_ptr;
    }

    /**
     * Returns the native window application instance pointer based on the current
     * platform. You will need to cast it to the appropriate native handle.
     * @return Pointer to the native window application instance handle.
     */
    [[nodiscard]] void *getNativeAppInstancePointer()
    {
        return m_native_app_ptr;
    }

private:
    const Window *m_parent = nullptr;
    std::string m_name = "Default Window";
    WindowSize m_size = DEFAULT_WINDOW_SIZE;
    WindowPosition m_position = DEFAULT_WINDOW_POS;
    bool m_focused = false;
    bool m_enabled = false;
    bool m_closed = false;

    WindowEventCallback m_native_callback = [&](const WindowEventType type, const WindowEventData &data) {
        onWindowEvent(type, data);
    };
    mutable std::vector<WindowEventCallback> m_listeners;

    void *m_native_ptr = nullptr;
    void *m_native_app_ptr = nullptr;

    void onWindowEvent(WindowEventType type, const WindowEventData &data);
};
}

#endif  // KIRANA_CORE_WINDOW_HPP