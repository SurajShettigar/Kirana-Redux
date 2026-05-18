// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_I_WINDOW_HPP
#define KIRANA_CORE_I_WINDOW_HPP

#include "window_constants.hpp"

namespace kirana::core
{
class IWindow : NoCopy
{
  public:
    IWindow() = default;
    virtual ~IWindow() = default;

    /// Create the native OS window.
    /// The native window implementation needs to call the base init function before implementing its own logic.
    /// @param desc            Creation parameters.
    /// @param handle          The logical handle this window is registered under (provided by Window Manager).
    /// @param event_manager   Pointer to a global event manager onto which current window events will be recorded. May
    ///                        be nullptr if the caller doesn't want events.
    /// @returns True if window was successfully created.
    virtual bool init(const WindowDesc &desc, const WindowHandle handle, EventManager *event_manager)
    {
        m_handle = handle;
        m_event_manager = event_manager;

        m_desc = desc;
        m_open = false;
        m_focused = false;
        m_minimized = false;

        return true;
    }

    /// Destroy the native OS window and release all resources.
    virtual void destroy()
    {
        m_desc.visible = false;
        m_open = false;
        m_focused = false;
        m_minimized = false;
    }

    /// Process all pending OS messages for this window. Must be called every frame from the main thread.
    virtual void pollEvents() = 0;

    [[nodiscard]] bool isOpen() const
    {
        return m_open;
    }
    [[nodiscard]] bool isFocused() const
    {
        return m_focused;
    }
    [[nodiscard]] bool isMinimized() const
    {
        return m_minimized;
    }
    [[nodiscard]] bool isResizable() const
    {
        return m_desc.resizable;
    }
    [[nodiscard]] bool isVisible() const
    {
        return m_desc.visible;
    }
    [[nodiscard]] WindowSize getSize() const
    {
        return m_desc.size;
    }
    [[nodiscard]] WindowPosition getPosition() const
    {
        return m_desc.position;
    }
    [[nodiscard]] const std::string &getTitle() const
    {
        return m_desc.title;
    }

    virtual void setTitle(const std::string &title)
    {
        m_desc.title = title;
    }
    virtual void resize(const WindowSize &size)
    {
        m_desc.size = size;
    }
    virtual void move(const WindowPosition &position)
    {
        m_desc.position = position;
    }
    virtual void show()
    {
        m_desc.visible = true;
    }
    virtual void hide()
    {
        m_desc.visible = false;
    }


    /// Low-level native handle access (Example use-case: For surface creation (Vulkan, OpenGL, etc.).)
    /// Cast to the correct type only in platform-specific code.
    ///
    ///   Win32:  <b>HWND</b> \n
    ///   X11:    <b>Window  (XID)</b> \n
    ///   macOS:  <b>NSWindow*</b> \n
    [[nodiscard]] virtual void *getNativeWindowHandle() const = 0;
    /// Low-level native handle access (Example use-case: For surface creation (Vulkan, OpenGL, etc.).)
    /// Cast to the correct type only in platform-specific code.
    ///
    ///   Win32:  <b>HINSTANCE</b> \n
    ///   X11:    <b>Display*</b> \n
    ///   macOS:  <b>nullptr</b>  (not needed on Cocoa) \n
    [[nodiscard]] virtual void *getNativeDisplayHandle() const = 0;

  protected:
    WindowHandle m_handle{};
    EventManager *m_event_manager{nullptr};

    WindowDesc m_desc{};
    bool m_open{false};
    bool m_focused{false};
    bool m_minimized{false};
};
} // namespace kirana::core

template <> struct std::hash<kirana::core::WindowHandle>
{
    std::size_t operator()(const kirana::core::WindowHandle &h) const noexcept
    {
        return h.m_value;
    }
};

#endif // KIRANA_CORE_I_WINDOW_HPP
