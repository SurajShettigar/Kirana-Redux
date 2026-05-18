// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WINDOW_MANAGER_HPP
#define KIRANA_CORE_WINDOW_MANAGER_HPP

#include "i_window.hpp"
#include "no_copy.hpp"

#include <memory>
#include <unordered_map>

namespace kirana::core
{
///  Creates, tracks, and destroys IWindow instances.
///  Every window is identified externally by an opaque WindowHandle.
///
///  Typical usage:
///   @code
///    EventManager  evt_mgr{EventDispatchMode::QUEUED};
///    WindowManager win_mgr;
///
///    auto main = win_mgr.createWindow({ .title="Main", .width=1280, .height=720 },
///                                    &evt_mgr);
///    auto hud  = win_mgr.createWindow({ .title="HUD",  .width=400,  .height=300 },
///                                    &evt_mgr);
///
///    while (win_mgr.hasOpenWindows())
///    {
///        win_mgr.pollAll();
///        evt_mgr.pollEvents();   // flush queued events to listeners
///    }
///    @endcode
class WindowManager : NoCopy, EventListener
{
  public:
    /// @param event_manager Optional EventManager to receive window + input events.
    explicit WindowManager(EventManager *event_manager = nullptr) : m_event_manager{event_manager}
    {
        if (m_event_manager)
        {
            m_event_manager->addListener(this);
        }
    }

    ~WindowManager() override
    {
        closeAll();
    }

    /// Creates a new window and returns its handle.
    /// @param desc     Window creation parameters.
    /// @returns Window handle. Handle will be invalid in case of failure.
    [[nodiscard]] WindowHandle createWindow(const WindowDesc &desc);

    /// Closes a window by the given handle.
    void closeWindow(WindowHandle handle) const;

    /// Closes all windows managed by this instance.
    void closeAll() const;

    /// Poll OS events for every open window.
    void pollAll();

    /// Poll OS events for a single window.
    void poll(WindowHandle handle) const;

    /// True if at least one window is still open (not closed by the OS/user).
    [[nodiscard]] bool hasOpenWindows() const;

    /// True if the given handle refers to a live window.
    [[nodiscard]] bool isValid(WindowHandle handle) const;

    [[nodiscard]] bool isOpen(WindowHandle handle) const;
    [[nodiscard]] bool isFocused(WindowHandle handle) const;
    [[nodiscard]] bool isMinimized(WindowHandle handle) const;
    [[nodiscard]] bool isResizable(WindowHandle handle) const;
    [[nodiscard]] bool isVisible(WindowHandle handle) const;
    [[nodiscard]] WindowSize getSize(WindowHandle handle) const;
    [[nodiscard]] WindowPosition getPosition(WindowHandle handle) const;
    [[nodiscard]] std::string getTitle(WindowHandle handle) const;

    void setTitle(WindowHandle handle, const std::string &title) const;
    void resize(WindowHandle handle, const WindowSize &size) const;
    void move(WindowHandle handle, const WindowPosition &position) const;
    void show(WindowHandle handle) const;
    void hide(WindowHandle handle) const;

    /// Low-level native handle access (Example use-case: For surface creation (Vulkan, OpenGL, etc.).)
    /// Cast to the correct type only in platform-specific code.
    ///
    ///   Win32:  <b>HWND</b> \n
    ///   X11:    <b>Window  (XID)</b> \n
    ///   macOS:  <b>NSWindow*</b> \n
    [[nodiscard]] void *getNativeWindowHandle(WindowHandle handle) const;
    /// Low-level native handle access (Example use-case: For surface creation (Vulkan, OpenGL, etc.).)
    /// Cast to the correct type only in platform-specific code.
    ///
    ///   Win32:  <b>HINSTANCE</b> \n
    ///   X11:    <b>Display*</b> \n
    ///   macOS:  <b>nullptr</b>  (not needed on Cocoa) \n
    [[nodiscard]] void *getNativeDisplayHandle(WindowHandle handle) const;

    void onEvent(const Event &event) override;

  private:
    EventManager *m_event_manager{nullptr};
    std::unordered_map<WindowHandle, std::unique_ptr<IWindow>> m_windows{};
    std::vector<WindowHandle> m_destroyed_windows{};
    WindowHandle m_next_handle{0};

    [[nodiscard]] IWindow *getWindow(WindowHandle handle) const;

    void pruneClosedWindows();
};

} // namespace kirana::core

#endif // KIRANA_CORE_WINDOW_MANAGER_HPP