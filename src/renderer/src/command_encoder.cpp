// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "command_encoder.hpp"

#include "helpers_vulkan.hpp"
#include "logger.hpp"

namespace kirana::renderer
{
bool CommandEncoder::init(const Device &device)
{
    m_device = device.m_device;
    // TODO: Fetch queue family index based on type of queue which will be used to submit this command encoder.
    const auto pool_create_info = vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                            device.getGraphicsQueueInfo().family_index};
    m_pool = m_device.createCommandPool(pool_create_info);

    const auto buffer_create_info = vk::CommandBufferAllocateInfo{m_pool, vk::CommandBufferLevel::ePrimary, 1};
    m_buffer = m_device.allocateCommandBuffers(buffer_create_info).front();

    m_fence = m_device.createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});
    return true;
}

void CommandEncoder::begin(const SwapchainTexture &render_target)
{
    if (const auto result = m_device.waitForFences({m_fence}, true, FENCE_WAIT_TIMEOUT); result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to wait for fence: " + vk::to_string(result));
    }
    m_device.resetFences({m_fence});

    m_buffer.reset();
    m_buffer.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
}

void CommandEncoder::finish()
{
    m_buffer.end();
}

void CommandEncoder::destroy()
{
    if (m_device)
    {
        if (m_fence)
        {
            m_device.destroyFence(m_fence);
            m_fence = nullptr;
        }
        if (m_pool)
        {
            m_device.destroyCommandPool(m_pool);
            m_pool = nullptr;
            m_buffer = nullptr;
        }
    }
}


}