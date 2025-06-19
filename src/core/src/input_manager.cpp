// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "input_manager.hpp"

#if PLATFORM == WINDOWS
#include "win32_input.hpp"
#else
// TODO: Add platform specific input management includes.
#endif

namespace kirana::core
{
bool InputManager::init()
{
#if PLATFORM == WINDOWS
    return initializeWin32Input();
#else
    // TODO: Add input initialization for other platforms.
#endif
}

void InputManager::pollInputs()
{
#if PLATFORM == WINDOWS
    getWin32MousePosition(m_mouse_pos);
    getWin32KeyStatus(m_keyboard_status, m_mouse_status);
#else
    // TODO: Add input polling for other platforms.
#endif
}
}