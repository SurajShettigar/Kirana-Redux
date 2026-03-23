// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "application.hpp"

#include "globals.hpp"

#include <logger.hpp>
#include <loader.hpp>

#include <algorithm>

namespace kirana
{
void Application::onWindowEvent(const core::WindowEventType type, const core::WindowEventData &data)
{
    if (type == core::WindowEventType::RESIZING)
    {
        const auto aspect_ratio = static_cast<float>(data.size.width) / static_cast<float>(data.size.height);
        m_scene.getActiveCamera()->setAspectRatio(aspect_ratio);
        m_renderer.updateCamera(m_scene.getViewMatrix(), m_scene.getProjectionMatrix());
        m_renderer.resize({static_cast<uint32_t>(data.size.width), static_cast<uint32_t>(data.size.height)});
    }
}


int Application::init()
{
    std::string app_name = APP_NAME;
    std::ranges::transform(app_name, app_name.begin(), ::toupper);


    core::Logger::get().init(app_name, core::Logger::Level::L_DEBUG);
    m_time_manager.init();

    core::InputManager::init();

    m_window_manager.init();
    m_main_window = m_window_manager.createWindow("Kirana", {1280, 720});
    m_window_manager.showWindow(m_main_window);

    m_window_manager.getWindow(m_main_window)
        .addOnWindowEventListener(
            [&](const core::WindowEventType type, const core::WindowEventData &data) { onWindowEvent(type, data); });

    core::Window &window = m_window_manager.getWindow(m_main_window);
    const renderer::SurfaceData surface{window.getNativeWindowPointer(), window.getNativeAppInstancePointer()};
    const renderer::SwapchainData swapchain{renderer::Size2D{window.getSize().width, window.getSize().height},
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
        const auto aspect_ratio =
            static_cast<float>(window.getSize().width) / static_cast<float>(window.getSize().height);
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
    m_window_manager.pollEvents();
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
        while (m_window_manager.isAnyWindowActive())
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