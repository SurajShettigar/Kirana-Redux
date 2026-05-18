// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "application.hpp"

#include "globals.hpp"

#include <logger.hpp>
#include <loader.hpp>

#include <algorithm>

namespace kirana
{
void Application::onEvent(const core::Event &event)
{
    if (event.getType() == core::WindowEvent::getStaticType())
    {
        const auto &win_event = dynamic_cast<const core::WindowEvent &>(event);
        if (win_event.status == core::WindowStatus::RESIZING)
        {
            const auto [width, height] = win_event.data.size;
            const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
            m_scene.getActiveCamera()->setAspectRatio(aspect_ratio);
            m_renderer.updateCamera(m_scene.getViewMatrix(), m_scene.getProjectionMatrix());
            m_renderer.resize({static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
        }
    }
}

int Application::init()
{
    std::string app_name = APP_NAME;
    std::ranges::transform(app_name, app_name.begin(), ::toupper);


    core::Logger::get().init(app_name, core::Logger::Level::L_DEBUG);

    m_event_manager.addListener(this);
    m_time_manager.init();

    core::InputManager::init();

    m_main_window = m_window_manager.createWindow(core::WindowDesc{"Kirana", {1280, 720}});

    void *native_win_handle = m_window_manager.getNativeWindowHandle(m_main_window);
    void *native_display_handle = m_window_manager.getNativeDisplayHandle(m_main_window);
    const core::WindowSize win_size = m_window_manager.getSize(m_main_window);

    const renderer::SurfaceData surface{native_win_handle, native_display_handle};
    const renderer::SwapchainData swapchain{renderer::Size2D{win_size.width, win_size.height},
                                            renderer::TextureFormat::B8G8R8A8_UNORM};

    renderer::GPUSelectionPreference gpu{renderer::GPUType::DISCRETE};
    gpu.features.shader_draw_parameters = true;
    gpu.features.synchronization_2 = true;
    gpu.features.dynamic_rendering = true;
    gpu.features.buffer_device_address = true;
    gpu.features.runtime_descriptor_array = true;
    gpu.features.descriptor_binding_partially_bound = true;
    // gpu.features.acceleration_structure = true;
    // gpu.features.ray_tracing_pipeline = true;
    if (!m_launch_scene_file.empty())
    {
        const auto info = scene::loadScene(m_launch_scene_file, &m_scene);
        if (!m_environment_file.empty())
        {
            m_scene.setEnvironmentLightImage(
                m_environment_file,
                scene::TextureSampler{scene::TextureFilterMode::LINEAR, scene::TextureFilterMode::LINEAR});
        }
        core::Logger::get().info("Loaded scene at path: " + info.path);
        const auto aspect_ratio = static_cast<float>(win_size.width) / static_cast<float>(win_size.height);
        if (!m_scene.getActiveCamera())
        {
            const auto cam_transform =
                scene::Transform{scene::Matrix4::lookAt(scene::Vector3{0.0f, 0.75f, 2.0f},
                                                        scene::Vector3{0.0f, 0.0f, 0.0f}, scene::Vector3::UP)
                                     .transformInverse()};
            const auto handle = m_scene.addPerspectiveCameraNode("Main_Camera", {0.01f, 1000.0f}, 30.0f, aspect_ratio,
                                                                 scene::NodeFlags::NONE, cam_transform);
            m_scene.setActiveCamera(handle);
        }
        m_scene.getActiveCamera()->setAspectRatio(aspect_ratio);
    }

    const bool is_initialized = m_renderer.init(
        renderer::DeviceInitializationData{true, app_name, APP_VERSION, surface, gpu}, swapchain, m_scene);

    m_is_cleaned = false;
    return is_initialized ? 0 : 1;
}

void Application::update()
{
    m_input_manager.pollInputs();
    m_window_manager.pollAll();
    m_event_manager.pollEvents();
    m_renderer.update();
}

void Application::render()
{
    m_renderer.render();
}

void Application::lateUpdate()
{
    m_renderer.lateUpdate();
}

void Application::clean()
{
    if (!m_is_cleaned)
    {
        m_renderer.clean();
        m_window_manager.closeWindow(m_main_window);
        m_is_cleaned = true;
    }
}

int Application::run()
{
    const int status = init();

    core::Logger::get().debug("Application Initialized with status code: " + std::to_string(status));

    if (!status)
    {
        while (m_window_manager.hasOpenWindows())
        {
            m_time_manager.tick([&]() {
                update();
                render();
                lateUpdate();
            });
        }
    }
    core::Logger::get().debug("Application Quitting...");
    clean();

    return status;
}
} // namespace kirana