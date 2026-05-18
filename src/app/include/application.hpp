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

class Application : core::NoCopy, core::EventListener
{
  public:
    Application() = default;

    explicit Application(const int argc, char **argv)
    {
        if (argc > 1)
        {
            m_launch_scene_file = argv[1];
        }
        if (argc > 2)
        {
            m_environment_file = argv[2];
        }
    }

    ~Application() override
    {
        clean();
    }

    int run();

    void onEvent(const core::Event &event) override;

  private:
    std::string m_launch_scene_file{};
    std::string m_environment_file{};

    core::EventManager m_event_manager{core::EventDispatchMode::QUEUED};
    core::TimeManager m_time_manager{};
    core::InputManager m_input_manager{};
    core::WindowManager m_window_manager{&m_event_manager};
    renderer::Renderer m_renderer{};

    core::WindowHandle m_main_window{};
    scene::Scene m_scene{};

    bool m_is_cleaned{false};

    int init();
    void update();
    void render();
    void lateUpdate();
    void clean();
};

} // namespace kirana

#endif // KIRANA_APPLICATION_HPP