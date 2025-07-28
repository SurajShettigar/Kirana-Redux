// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "application.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

#include "globals.hpp"

#include <logger.hpp>

namespace kirana
{
void Application::onWindowEvent(const core::WindowEventType type, const core::WindowEventData &data)
{
    if (type == core::WindowEventType::RESIZING)
    {
        m_renderer.resize({static_cast<uint32_t>(data.size.width), static_cast<uint32_t>(data.size.height)});
    }
}


int Application::init()
{
    std::string app_name = std::string(APP_NAME);
    std::transform(app_name.begin(), app_name.end(), app_name.begin(), ::toupper);


    core::Logger::get().init(app_name, core::Logger::Level::L_DEBUG);
    m_time_manager.init();

    m_input_manager.init();

    m_window_manager.init();
    m_main_window = m_window_manager.createWindow("Kirana", {1280, 720});
    m_window_manager.showWindow(m_main_window);

    m_window_manager.getWindow(m_main_window).addOnWindowEventListener(
        [&](const core::WindowEventType type, const core::WindowEventData &data) {
            onWindowEvent(type, data);
        });

    core::Window &window = m_window_manager.getWindow(m_main_window);
    const renderer::SurfaceData surface{window.getNativeWindowPointer(), window.getNativeAppInstancePointer()};
    const renderer::SwapchainData swapchain{renderer::Size2D{window.getSize().width, window.getSize().height}};

    renderer::GPUSelectionPreference gpu{renderer::GPUType::DISCRETE};
    gpu.features.synchronization_2 = true;
    gpu.features.dynamic_rendering = true;
    gpu.features.buffer_device_address = true;
    // gpu.features.acceleration_structure = true;
    // gpu.features.ray_tracing_pipeline = true;
    const bool is_initialized = m_renderer.
        init(renderer::DeviceInitializationData{true, app_name, APP_VERSION, surface, gpu}, swapchain);

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

    core::Logger::get().debug("Application Initialized with status code: " +
                              std::to_string(status));

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
}