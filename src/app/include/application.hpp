// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_APPLICATION_HPP
#define KIRANA_APPLICATION_HPP

#include <no_copy.hpp>
#include <time_manager.hpp>
#include <input_manager.hpp>
#include <window_manager.hpp>
#include <renderer.hpp>

namespace kirana
{

class Application : core::NoCopy
{
public:
    Application() = default;

    explicit Application(int argc, char **argv)
    {
    }

    ~Application()
    {
        clean();
    }

    int run();

private:
    core::TimeManager m_time_manager = {};
    core::InputManager m_input_manager = {};
    core::WindowManager m_window_manager = {};
    renderer::Renderer m_renderer = {};

    core::Handle<core::Window> m_main_window = {};

    int init();
    void update();
    void render();
    void lateUpdate();
    void clean();
};

} // namespace kirana

#endif  // KIRANA_APPLICATION_HPP