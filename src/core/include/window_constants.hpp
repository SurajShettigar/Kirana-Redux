// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WINDOW_CONSTANTS_HPP
#define KIRANA_CORE_WINDOW_CONSTANTS_HPP

#include <cstdint>
#include <functional>
#include <string>

#include "event_manager.hpp"

namespace kirana::core
{
/// An integer-based unique identifier associated with a window resource.
class WindowHandle final
{
    friend class std::hash<WindowHandle>;

  public:
    WindowHandle() = default;
    ~WindowHandle() = default;

    explicit WindowHandle(const uint32_t value) : m_value{value}
    {
    }

    [[nodiscard]] bool isValid() const
    {
        return m_value != std::numeric_limits<uint32_t>::max();
    }

    explicit operator uint32_t() const
    {
        return m_value;
    }

    bool operator==(const WindowHandle &other) const
    {
        return m_value == other.m_value;
    }

    explicit operator bool() const
    {
        return isValid();
    }

    WindowHandle &operator++()
    {
        if (isValid())
        {
            ++m_value;
        }
        return *this;
    }

    WindowHandle operator++(int)
    {
        const WindowHandle tmp = *this;
        if (isValid())
        {
            ++m_value;
        }
        return tmp;
    }

  private:
    uint32_t m_value{std::numeric_limits<uint32_t>::max()};
};

struct WindowSize
{
    uint16_t width;
    uint16_t height;
};

struct WindowPosition
{
    int32_t x;
    int32_t y;
};

struct WindowDesc
{
    std::string title = "Window";
    WindowSize size{1280, 720};
    WindowPosition position{-1, -1};
    bool resizable = true;
    bool visible = true;
};

enum class WindowStatus
{
    /// Undefined event type.
    UNKNOWN = 0,
    /// When the window changes focus (selected / made active).
    FOCUSED = 1,
    /// When the window receives a signal to close. Called before the window is
    /// closed, so the user can decide to show confirmation dialog before closing.
    CLOSING = 2,
    /// When the window is created. The event is sent just before the window
    /// becomes visible.
    CREATED = 3,
    /// When the window has finished closing and is destroyed. The event is sent
    /// after the window is removed from the screen.
    DESTROYED = 4,
    /// When the window changes its enabled state. Enabled windows receive input
    /// events, and disabled windows do not.
    ENABLED = 5,
    /// When the window is being moved. Called multiple times as long as the
    /// window is moving.
    MOVING = 6,
    /// When the window has started moving or resizing. Called once at the start.
    MOVING_RESIZING_START = 7,
    /// When the window has finished moving or resizing. Called once at the end.
    MOVING_RESIZING_END = 8,
    /// When the window is being resized. Called multiple times as long as the
    /// window is resizing.
    RESIZING = 9,
    /// When the window is minimized (window dimensions are 0, 0).
    MINIMIZED = 10,
    COUNT = 11
};

struct WindowEventData
{
    union {
        bool focused;
        bool enabled;
        WindowSize size;
        WindowPosition position;
    };
};

///  Single event type for all window state changes, discriminated by `status`.
///  The `data` union is only meaningful for statuses that carry a payload:
///
///    FOCUSED               → data.focused   (true = gained, false = lost)
///    ENABLED               → data.enabled   (true = enabled, false = disabled)
///    RESIZING              → data.size
///    MOVING                → data.position
///    CREATED / CLOSING /
///    CLOSED / MINIMIZE / MOVING_RESIZING_START / MOVING_RESIZING_END
///                          → data unused; check handle + status only
///
///  The window handle is always valid so listeners can identify which window
///  the event belongs to.
struct WindowEvent : Event
{
    WindowHandle handle{};
    WindowStatus status{WindowStatus::UNKNOWN};
    WindowEventData data{};

    WindowEvent(const WindowHandle h, const WindowStatus s) : handle{h}, status{s}
    {
    }

    static WindowEvent focused(const WindowHandle h, const bool gained)
    {
        WindowEvent e{h, WindowStatus::FOCUSED};
        e.data.focused = gained;
        return e;
    }

    static WindowEvent enabled(const WindowHandle h, const bool is_enabled)
    {
        WindowEvent e{h, WindowStatus::ENABLED};
        e.data.enabled = is_enabled;
        return e;
    }

    static WindowEvent resizing(const WindowHandle h, const uint16_t w, const uint16_t ht)
    {
        WindowEvent e{h, WindowStatus::RESIZING};
        e.data.size = {w, ht};
        return e;
    }

    static WindowEvent moving(const WindowHandle h, const int32_t x, const int32_t y)
    {
        WindowEvent e{h, WindowStatus::MOVING};
        e.data.position = {x, y};
        return e;
    }

    DEFINE_EVENT_TYPE(WindowEvent)
};
} // namespace kirana::core

#endif // KIRANA_CORE_WINDOW_CONSTANTS_HPP