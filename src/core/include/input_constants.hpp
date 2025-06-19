// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_INPUT_CONSTANTS_HPP
#define KIRANA_CORE_INPUT_CONSTANTS_HPP

#include <cstdint>

namespace kirana::core
{
/// Input button action
enum class KeyAction
{
    /// No Key action.
    NONE = 0,
    /// Key released.
    UP = 1,
    /// Key pressed.
    DOWN = 2,
    /// Key held down.
    HELD = 3,
    /// Key double-clicked.
    DOUBLE_CLICK = 4,
    COUNT = 5
};

/// Mouse buttons
enum class MouseKey
{
    UNKNOWN = 0,
    LEFT = 1,
    RIGHT = 2,
    MIDDLE = 3,
    BUTTON_01 = 4,
    BUTTON_02 = 5,
    COUNT = 6
};

/// Keyboard buttons
enum class KeyboardKey
{
    UNKNOWN = 0,
    BACKSPACE,
    TAB,
    ENTER,
    LEFT_SHIFT,
    LEFT_CTRL,
    LEFT_OS,
    LEFT_ALT,
    RIGHT_SHIFT,
    RIGHT_CTRL,
    RIGHT_OS,
    RIGHT_ALT,
    PAUSE,
    CAPS_LOCK,
    NUM_LOCK,
    SCROLL_LOCK,
    ESCAPE,
    SPACE,
    PAGE_UP,
    PAGE_DOWN,
    END,
    HOME,
    LEFT_ARROW,
    UP_ARROW,
    RIGHT_ARROW,
    DOWN_ARROW,
    PRINT_SCREEN,
    INSERT,
    DELETE,
    HELP,

    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,

    NUMPAD_0,
    NUMPAD_1,
    NUMPAD_2,
    NUMPAD_3,
    NUMPAD_4,
    NUMPAD_5,
    NUMPAD_6,
    NUMPAD_7,
    NUMPAD_8,
    NUMPAD_9,
    NUMPAD_DIVIDE,
    NUMPAD_MULTIPLY,
    NUMPAD_SUBTRACT,
    NUMPAD_ADD,
    NUMPAD_DECIMAL,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    /// = +
    EQUAL,
    /// - _
    MINUS,
    /// [ {
    LEFT_BRACKET,
    /// ] }
    RIGHT_BRACKET,
    /// \ |
    BACKWARD_SLASH,
    /// ; :
    SEMI_COLON,
    /// ' "
    APOSTROPHE,
    /// , <
    COMMA,
    /// . >
    PERIOD,
    /// / ?
    FORWARD_SLASH,
    /// ` ~
    TILDE,
    COUNT
};

/// Special keys used in
enum class ModifierKey
{
    NONE = 0,
    SHIFT = 1 << 1,
    CTRL = 1 << 2,
    ALT = 1 << 3,
    OS = 1 << 4,
    CAPS_LOCK = 1 << 5,
    NUM_LOCK = 1 << 6
};

struct MouseInput
{
    MouseKey key;
    ModifierKey modifier;
    KeyAction action;
};

struct KeyboardInput
{
    KeyboardKey key;
    KeyAction action;
};

struct MousePosition
{
    uint16_t x;
    uint16_t y;
};

inline ModifierKey operator|(ModifierKey lhs, ModifierKey rhs)
{
    return static_cast<ModifierKey>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline ModifierKey operator&(ModifierKey lhs, ModifierKey rhs)
{
    return static_cast<ModifierKey>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline ModifierKey operator^(ModifierKey lhs, ModifierKey rhs)
{
    return static_cast<ModifierKey>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

inline ModifierKey operator~(ModifierKey key)
{
    return static_cast<ModifierKey>(~static_cast<uint8_t>(key));
}
}

#endif  // KIRANA_CORE_INPUT_CONSTANTS_HPP