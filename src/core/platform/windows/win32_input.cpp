// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "win32_input.hpp"

#include <windows.h>

namespace kirana::core
{

static const std::unordered_map<KeyboardKey, int> WIN32_KEYBOARD_MAPPING = {
    {KeyboardKey::BACKSPACE, VK_BACK},
    {KeyboardKey::TAB, VK_TAB},
    {KeyboardKey::ENTER, VK_RETURN},
    {KeyboardKey::LEFT_SHIFT, VK_LSHIFT},
    {KeyboardKey::LEFT_CTRL, VK_LCONTROL},
    {KeyboardKey::LEFT_OS, VK_LWIN},
    {KeyboardKey::LEFT_ALT, VK_LMENU},
    {KeyboardKey::RIGHT_SHIFT, VK_RSHIFT},
    {KeyboardKey::RIGHT_CTRL, VK_RCONTROL},
    {KeyboardKey::RIGHT_OS, VK_RWIN},
    {KeyboardKey::RIGHT_ALT, VK_RMENU},
    {KeyboardKey::PAUSE, VK_PAUSE},
    {KeyboardKey::CAPS_LOCK, VK_CAPITAL},
    {KeyboardKey::NUM_LOCK, VK_NUMLOCK},
    {KeyboardKey::SCROLL_LOCK, VK_SCROLL},
    {KeyboardKey::ESCAPE, VK_ESCAPE},
    {KeyboardKey::SPACE, VK_SPACE},
    {KeyboardKey::PAGE_UP, VK_PRIOR},
    {KeyboardKey::PAGE_DOWN, VK_NEXT},
    {KeyboardKey::END, VK_END},
    {KeyboardKey::HOME, VK_HOME},
    {KeyboardKey::LEFT_ARROW, VK_LEFT},
    {KeyboardKey::UP_ARROW, VK_UP},
    {KeyboardKey::RIGHT_ARROW, VK_RIGHT},
    {KeyboardKey::DOWN_ARROW, VK_DOWN},
    {KeyboardKey::PRINT_SCREEN, VK_SNAPSHOT},
    {KeyboardKey::INSERT, VK_INSERT},
#ifdef DELETE
#undef DELETE
    {KeyboardKey::DELETE, VK_DELETE},
#define DELETE (0x00010000L)
#endif

    {KeyboardKey::HELP, VK_HELP},
    {KeyboardKey::NUM_0, 0x30},
    {KeyboardKey::NUM_1, 0x31},
    {KeyboardKey::NUM_2, 0x32},
    {KeyboardKey::NUM_3, 0x33},
    {KeyboardKey::NUM_4, 0x34},
    {KeyboardKey::NUM_5, 0x35},
    {KeyboardKey::NUM_6, 0x36},
    {KeyboardKey::NUM_7, 0x37},
    {KeyboardKey::NUM_8, 0x38},
    {KeyboardKey::NUM_9, 0x39},

    {KeyboardKey::NUMPAD_0, VK_NUMPAD0},
    {KeyboardKey::NUMPAD_1, VK_NUMPAD1},
    {KeyboardKey::NUMPAD_2, VK_NUMPAD2},
    {KeyboardKey::NUMPAD_3, VK_NUMPAD3},
    {KeyboardKey::NUMPAD_4, VK_NUMPAD4},
    {KeyboardKey::NUMPAD_5, VK_NUMPAD5},
    {KeyboardKey::NUMPAD_6, VK_NUMPAD6},
    {KeyboardKey::NUMPAD_7, VK_NUMPAD7},
    {KeyboardKey::NUMPAD_8, VK_NUMPAD8},
    {KeyboardKey::NUMPAD_9, VK_NUMPAD9},
    {KeyboardKey::NUMPAD_DIVIDE, VK_DIVIDE},
    {KeyboardKey::NUMPAD_MULTIPLY, VK_MULTIPLY},
    {KeyboardKey::NUMPAD_SUBTRACT, VK_SUBTRACT},
    {KeyboardKey::NUMPAD_ADD, VK_ADD},
    {KeyboardKey::NUMPAD_DECIMAL, VK_DECIMAL},

    {KeyboardKey::A, 0x41},
    {KeyboardKey::B, 0x42},
    {KeyboardKey::C, 0x43},
    {KeyboardKey::D, 0x44},
    {KeyboardKey::E, 0x45},
    {KeyboardKey::F, 0x46},
    {KeyboardKey::G, 0x47},
    {KeyboardKey::H, 0x48},
    {KeyboardKey::I, 0x49},
    {KeyboardKey::J, 0x4A},
    {KeyboardKey::K, 0x4B},
    {KeyboardKey::L, 0x4C},
    {KeyboardKey::M, 0x4D},
    {KeyboardKey::N, 0x4E},
    {KeyboardKey::O, 0x4F},
    {KeyboardKey::P, 0x50},
    {KeyboardKey::Q, 0x51},
    {KeyboardKey::R, 0x52},
    {KeyboardKey::S, 0x53},
    {KeyboardKey::T, 0x54},
    {KeyboardKey::U, 0x55},
    {KeyboardKey::V, 0x56},
    {KeyboardKey::W, 0x57},
    {KeyboardKey::X, 0x58},
    {KeyboardKey::Y, 0x59},
    {KeyboardKey::Z, 0x5A},

    {KeyboardKey::F1, VK_F1},
    {KeyboardKey::F2, VK_F2},
    {KeyboardKey::F3, VK_F3},
    {KeyboardKey::F4, VK_F4},
    {KeyboardKey::F5, VK_F5},
    {KeyboardKey::F6, VK_F6},
    {KeyboardKey::F7, VK_F7},
    {KeyboardKey::F8, VK_F8},
    {KeyboardKey::F9, VK_F9},
    {KeyboardKey::F10, VK_F10},
    {KeyboardKey::F11, VK_F11},
    {KeyboardKey::F12, VK_F12},

    {KeyboardKey::EQUAL, VK_OEM_PLUS},
    {KeyboardKey::MINUS, VK_OEM_MINUS},
    {KeyboardKey::LEFT_BRACKET, VK_OEM_4},
    {KeyboardKey::RIGHT_BRACKET, VK_OEM_6},
    {KeyboardKey::BACKWARD_SLASH, VK_OEM_5},
    {KeyboardKey::SEMI_COLON, VK_OEM_1},
    {KeyboardKey::APOSTROPHE, VK_OEM_7},
    {KeyboardKey::COMMA, VK_OEM_COMMA},
    {KeyboardKey::PERIOD, VK_OEM_PERIOD},
    {KeyboardKey::FORWARD_SLASH, VK_OEM_2},
    {KeyboardKey::TILDE, VK_OEM_3}
};

static const std::unordered_map<MouseKey, int> WIN32_MOUSE_MAPPING = {
    {MouseKey::LEFT, VK_LBUTTON},
    {MouseKey::RIGHT, VK_RBUTTON},
    {MouseKey::MIDDLE, VK_MBUTTON},
    {MouseKey::BUTTON_01, VK_XBUTTON1},
    {MouseKey::BUTTON_02, VK_XBUTTON2}
};

static LRESULT CALLBACK InputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INPUT)
    {
        auto hardware = reinterpret_cast<HRAWINPUT>(lParam);
        UINT data_size;
        GetRawInputData(hardware, RID_INPUT, nullptr, &data_size, sizeof(RAWINPUTHEADER));
        if (data_size == 0)
            return 0;

        auto data = new BYTE[data_size];
        if (GetRawInputData(hardware, RID_INPUT, data, &data_size, sizeof(RAWINPUTHEADER)) != data_size)
        {
            delete[] data;
            return 0;
        }

        const auto input = reinterpret_cast<const PRAWINPUT>(data);
        if (input->header.dwType == RIM_TYPEMOUSE)
        {
            const RAWMOUSE &mouse = input->data.mouse;
            // TODO: Implement raw mouse data extraction.
        }
        delete[] data;
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool initializeWin32Input()
{
    HINSTANCE instance = GetModuleHandle(nullptr);
    LPCSTR class_name = "InputMessageWindow";

    // Register input window class.
    WNDCLASS w_class = {};
    w_class.hCursor = LoadCursor(instance, IDC_ARROW);
    w_class.hInstance = instance;
    w_class.lpfnWndProc = InputWindowProc;
    w_class.lpszClassName = class_name;

    bool status = RegisterClass(&w_class) != 0;
    if (!status)
        return false;

    // Create input message only window.
    HWND win_handle = CreateWindowEx(0, class_name, class_name, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr);

    status = win_handle != nullptr;
    if (!status)
        return false;

    // Register raw input devices
    RAWINPUTDEVICE devices[1];
    // Mouse
    devices[0].usUsagePage = 0x01; // HID_USAGE_PAGE_GENERIC
    devices[0].usUsage = 0x02; // HID_USAGE_GENERIC_MOUSE
    devices[0].dwFlags = 0x00;
    devices[0].hwndTarget = win_handle;

    status = RegisterRawInputDevices(devices, 1, sizeof(devices[0])) != FALSE;

    return status;
}

void pollWin32InputEvents()
{
    MSG msg = {};
    HWND handle = FindWindowEx(HWND_MESSAGE, nullptr, "InputMessageWindow", "InputMessageWindow");
    if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool getWin32KeyStatus(std::unordered_map<KeyboardKey, KeyAction> &keyboard_status,
                              std::unordered_map<MouseKey, KeyAction> &mouse_status)
{
    BYTE key_state[256];
    if (!GetKeyboardState(key_state))
        return false;

    /// Ensures the key has correct state. Windows API gives only UP and DOWN
    /// state and the state is maintained every frame. The following function
    /// ensures DOWN and UP state are set only once at the start and end of key
    /// press respectively. Its state is transitioned to HELD or NONE respectively
    /// there after.
    const auto getKeyAction = [](const KeyAction &prev_action,
                                 const BYTE &curr_key_state) -> KeyAction {
        // Convert key state to action enum
        const KeyAction curr_action =
            (curr_key_state >> 7) & 0x01 ? KeyAction::DOWN : KeyAction::UP;

        // If the key has not been pressed or if the key was never pressed, set the
        // action to none.
        if (curr_action == KeyAction::UP &&
            (prev_action == KeyAction::UP || prev_action == KeyAction::NONE))
            return KeyAction::NONE;

        // If the key is down, and if the key was previously down, transition
        // the state to HELD state.
        if (curr_action == KeyAction::DOWN &&
            (prev_action == KeyAction::DOWN || prev_action == KeyAction::HELD))
            return KeyAction::HELD;

        return curr_action;
    };

    int v_key;
    for (auto &[key, action] : keyboard_status)
    {
        if (WIN32_KEYBOARD_MAPPING.count(key) == 0)
            continue;
        v_key = WIN32_KEYBOARD_MAPPING.at(key);
        action = getKeyAction(action, key_state[v_key]);
    }
    for (auto &[key, action] : mouse_status)
    {
        if (WIN32_MOUSE_MAPPING.count(key) == 0)
            continue;
        v_key = WIN32_MOUSE_MAPPING.at(key);
        action = getKeyAction(action, key_state[v_key]);
    }
    return true;
}

bool getWin32MousePosition(MousePosition &mousePosition)
{
    POINT point;
    if (!GetCursorPos(&point))
        return false;

    mousePosition.x = static_cast<uint16_t>(point.x);
    mousePosition.y = static_cast<uint16_t>(point.y);
    return true;
}
}
