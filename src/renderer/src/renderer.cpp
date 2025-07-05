// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool Renderer::init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data)
{
    bool status = m_device.init(init_data);
    m_fence = m_device.createFence();
    if (status && init_data.surface.isValid())
    {
        m_swapchain = m_device.createSwapchain(swapchain_data);
        status = m_swapchain.isValid();
    }
    if (status)
    {
        const bool has_swapchain = m_swapchain.isValid();
        const auto num_ctxs = has_swapchain
                                  ? m_swapchain.getTextureCount()
                                  : static_cast<size_t>(swapchain_data.buffer_mode);
        m_ctxs.reserve(num_ctxs);
        for (size_t i = 0; i < num_ctxs; ++i)
        {
            const auto encoder = m_device.createCommandEncoder(m_device.getGraphicsQueue());
            const auto render_semaphore = m_device.createSemaphore(PipelineStageFlags::ALL_GRAPHICS);
            Semaphore swapchain_semaphore;
            if (has_swapchain)
            {
                swapchain_semaphore = m_device.createSemaphore(PipelineStageFlags::COLOR_ATTACHMENT_OUTPUT);
            }
            m_ctxs.emplace_back(RenderContext{encoder, swapchain_semaphore, render_semaphore});
        }
    }
    return status;
}

void Renderer::update()
{
}

void Renderer::render()
{
    if (!m_fence.wait(FENCE_WAIT_TIMEOUT))
    {
        return;
    }
    m_fence.reset();

    const auto &[encoder, swapchain_semaphore, render_semaphore] = m_ctxs[m_current_index];
    Queue &queue = m_device.getGraphicsQueue();
    auto &render_target = m_swapchain.getTexture(swapchain_semaphore);

    if (swapchain_semaphore.isValid())
    {
        queue.addWaitSemaphore(swapchain_semaphore);
    }
    queue.addSignalSemaphore(render_semaphore);

    encoder.begin();
    encoder.transitionTextureLayout(render_target, TextureLayout::GENERAL);
    encoder.clearTexture(render_target, {1.0f, 0.0f, 0.0f, 1.0f});
    encoder.transitionTextureLayout(render_target, TextureLayout::PRESENT_SRC);

    queue.submit(encoder.finish(), m_fence);

    queue.addWaitSemaphore(render_semaphore);
    queue.present(m_swapchain.present());

    m_current_index = (m_current_index + 1) % m_ctxs.size();
}

void Renderer::lateUpdate()
{

}

void Renderer::clean()
{
    if (!m_ctxs.empty())
    {
        for (auto &[encoder, swapchain_semaphore, render_semaphore] : m_ctxs)
        {
            swapchain_semaphore.destroy();
            render_semaphore.destroy();
            encoder.destroy();
        }
        m_ctxs.clear();
    }
    m_swapchain.destroy();
    m_fence.destroy();
    m_device.destroy();
}
}