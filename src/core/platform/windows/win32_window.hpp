// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WIN32_WINDOW_HPP
#define KIRANA_CORE_WIN32_WINDOW_HPP

#include "i_window.hpp"

#include <windows.h>

namespace kirana::core
{
class Win32Window final : public IWindow
{
  public:
    Win32Window() = default;
    ~Win32Window() override
    {
        destroy();
    }

    [[nodiscard]] bool init(const WindowDesc &desc, WindowHandle handle, EventManager *event_manager) override;

    void destroy() override;

    void pollEvents() override;

    void setTitle(const std::string &title) override;
    void resize(const WindowSize &size) override;
    void move(const WindowPosition &position) override;
    void show() override;
    void hide() override;


    [[nodiscard]] void *getNativeWindowHandle() const override
    {
        return m_hwnd;
    }
    [[nodiscard]] void *getNativeDisplayHandle() const override
    {
        return m_hinstance;
    }

  private:
    HWND m_hwnd = nullptr;
    HINSTANCE m_hinstance = nullptr;

    static bool registerWindowClass(HINSTANCE hinstance);
    static void unregisterWindowClass(HINSTANCE hinstance);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // ── Helpers ───────────────────────────────────────────────────────────
    // static KeyboardKey translateVirtualKey(WPARAM vk, LPARAM lParam);
    // static ModifierKey getCurrentModifiers();
};
} // namespace kirana::core

#endif // KIRANA_CORE_WIN32_WINDOW_HPP
