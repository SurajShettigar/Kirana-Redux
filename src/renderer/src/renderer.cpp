// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

namespace kirana::renderer
{
bool Renderer::init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data)
{
    bool status = m_device.init(init_data);
    if (status && init_data.surface.isValid())
    {
        status = m_swapchain.init(m_device, swapchain_data);
    }
    if (status)
    {
        const auto num_encoders = m_swapchain.isValid()
                                      ? m_swapchain.getTextureCount()
                                      : static_cast<size_t>(swapchain_data.buffer_mode);
        m_cmd_encoders_graphic.reserve(num_encoders);
        for (size_t i = 0; i < num_encoders; ++i)
        {
            m_cmd_encoders_graphic.emplace_back();
            m_cmd_encoders_graphic.back().init(m_device);
        }
    }
    return status;
}

void Renderer::update()
{
}

void Renderer::render()
{
    auto &encoder = m_cmd_encoders_graphic[m_current_index];
    const auto render_target = m_swapchain.getTexture();
    encoder.begin(render_target);
    encoder.finish();
    // TODO: Queue submit
    m_swapchain.present();
    m_current_index = (m_current_index + 1) % m_cmd_encoders_graphic.size();
}

void Renderer::lateUpdate()
{
}

void Renderer::clean()
{
    if (!m_cmd_encoders_graphic.empty())
    {
        for (auto &ce : m_cmd_encoders_graphic)
        {
            ce.destroy();
        }
        m_cmd_encoders_graphic.clear();
    }
    if (m_swapchain.isValid())
    {
        m_swapchain.destroy();
    }
    if (m_device.isValid())
    {
        m_device.destroy();
    }
}
}