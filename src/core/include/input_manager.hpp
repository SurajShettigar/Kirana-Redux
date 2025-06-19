// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_INPUT_MANAGER_HPP
#define KIRANA_CORE_INPUT_MANAGER_HPP

#include <unordered_map>

#include "input_constants.hpp"
#include "no_copy.hpp"

namespace kirana::core
{
class InputManager : NoCopy
{
public:
    /**
     * Any input of the application from the input devices (Keyboard, Mouse, and
     * so on.) is managed by the InputManager.
     */
    InputManager() : m_mouse_pos{0, 0}
    {
        for (size_t i = 0; i < static_cast<size_t>(KeyboardKey::COUNT); i++)
            m_keyboard_status[static_cast<KeyboardKey>(i)] = KeyAction::NONE;
        for (size_t i = 0; i < static_cast<size_t>(MouseKey::COUNT); i++)
            m_mouse_status[static_cast<MouseKey>(i)] = KeyAction::NONE;
    };
    ~InputManager() = default;

    /**
     * Initializes the native input functions.
     * @return
     */
    static bool init();

    /**
     * Polls the input events. Needs to be called every frame.
     */
    void pollInputs();

    /// Returns true if the given keyboard key is pressed down. Returns true once
    /// at the start of the key press and false thereafter.
    [[nodiscard]] bool isKeyDown(const KeyboardKey key) const
    {
        return checkKeyStatus(key, KeyAction::DOWN);
    }

    /// Returns true if the given keyboard key is held down. Returns true as long
    /// as the key is held down.
    [[nodiscard]] bool isKeyHeld(const KeyboardKey key) const
    {
        return checkKeyStatus(key, KeyAction::HELD);
    }

    /// Returns true if the given keyboard key is released. Returns true once at
    /// the end of the key press and false thereafter.
    [[nodiscard]] bool isKeyUp(const KeyboardKey key) const
    {
        return checkKeyStatus(key, KeyAction::UP);
    }

    /// Returns true if the given mouse key is pressed down. Returns true once at
    /// the start of the key press and false thereafter.
    [[nodiscard]] bool isMouseDown(const MouseKey key) const
    {
        return checkMouseStatus(key, KeyAction::DOWN);
    }

    /// Returns true if the given mouse key is held down. Returns true as long
    /// as the key is held down.
    [[nodiscard]] bool isMouseHeld(const MouseKey key) const
    {
        return checkMouseStatus(key, KeyAction::HELD);
    }

    /// Returns true if the given mouse key is released. Returns true once at the
    /// end of the key press and false thereafter.
    [[nodiscard]] bool isMouseUp(const MouseKey key) const
    {
        return checkMouseStatus(key, KeyAction::UP);
    }

    /// Returns the current cursor position in screen-coordinates. (0,0)
    /// corresponds to the top-left corner of the screen.
    [[nodiscard]] const MousePosition &getMousePosition() const
    {
        return m_mouse_pos;
    }

    /// Returns the current cursor position in screen-coordinates. (0,0)
    /// corresponds to the top-left corner of the screen.
    [[nodiscard]] MousePosition getMousePosition()
    {
        return m_mouse_pos;
    }

private:
    MousePosition m_mouse_pos;
    std::unordered_map<KeyboardKey, KeyAction> m_keyboard_status;
    std::unordered_map<MouseKey, KeyAction> m_mouse_status;

    [[nodiscard]] bool checkKeyStatus(const KeyboardKey key, const KeyAction action) const
    {
        return m_keyboard_status.at(key) == action;
    }

    [[nodiscard]] bool checkMouseStatus(const MouseKey key, const KeyAction action) const
    {
        return m_mouse_status.at(key) == action;
    }
};
}

#endif  // KIRANA_CORE_INPUT_MANAGER_HPP