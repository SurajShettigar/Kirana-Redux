// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

namespace kirana::renderer
{
bool Renderer::init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data)
{
    bool status = m_device.init(init_data);
    if (status)
    {
        status = m_swapchain.init(m_device, swapchain_data);
    }
    return status;
}

void Renderer::update()
{
}

void Renderer::render()
{
}

void Renderer::lateUpdate()
{
}

void Renderer::clean()
{
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