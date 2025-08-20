// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WINDOW_CONSTANTS_HPP
#define KIRANA_CORE_WINDOW_CONSTANTS_HPP

#include <cstdint>
#include <functional>
#include <string>

#include <handle.hpp>

namespace kirana::core
{
class Window;

struct WindowSize
{
    uint16_t width;
    uint16_t height;
};

struct WindowPosition
{
    uint16_t x;
    uint16_t y;
};

enum class WindowEventType
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
    /// When the window has finised closing and is destroyed. The event is sent
          /// after the window is removed from the screen.
    CLOSED = 4,
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
    COUNT = 10
};

struct WindowEventData
{
    union
    {
        bool focused;
        bool enabled;
        WindowSize size;
        WindowPosition position;
    };
};

typedef std::function<void(WindowEventType type, const WindowEventData &data)> WindowEventCallback;

static const std::string WINDOW_EVENT_CLASS_NAME = "WindowManager";
static constexpr WindowSize DEFAULT_WINDOW_SIZE = {640, 360};
static constexpr WindowPosition DEFAULT_WINDOW_POS = {0, 0};

struct WindowTag {};
using WindowHandle = Handle<WindowTag>;
}

#endif  // KIRANA_CORE_WINDOW_CONSTANTS_HPP