// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_WIN32_INPUT_HPP
#define KIRANA_CORE_WIN32_INPUT_HPP

#include "input_constants.hpp"

#include <unordered_map>

namespace kirana::core
{
bool initializeWin32Input();
void pollWin32InputEvents();
bool getWin32MousePosition(MousePosition &mousePosition);
bool getWin32KeyStatus(std::unordered_map<KeyboardKey, KeyAction> &keyboard_status,
                       std::unordered_map<MouseKey, KeyAction> &mouse_status);
} // namespace kirana::core

#endif // KIRANA_CORE_WIN32_INPUT_HPP
