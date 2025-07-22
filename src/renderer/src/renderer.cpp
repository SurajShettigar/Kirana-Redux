// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

#include "helpers_vulkan.hpp"
#include "logger.hpp"

namespace kirana::renderer
{
bool Renderer::init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data)
{
    bool status = m_device.init(init_data);
    if (status && init_data.surface.isValid())
    {
        m_swapchain = m_device.createSwapchain("Swapchain", swapchain_data);
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
            const auto fence = m_device.createFence("Fence_Main_" + std::to_string(i));
            const auto encoder = m_device.createCommandEncoder("Encoder_Render_" + std::to_string(i),
                                                               m_device.getGraphicsQueue());
            const auto render_semaphore = m_device.createSemaphore("Semaphore_Render_" + std::to_string(i));
            Semaphore swapchain_semaphore;
            if (has_swapchain)
            {
                swapchain_semaphore = m_device.createSemaphore("Semaphore_Swapchain_" + std::to_string(i),
                                                               PipelineStageFlags::COLOR_ATTACHMENT_OUTPUT);
            }
            m_ctxs.emplace_back(RenderContext{fence, encoder, swapchain_semaphore, render_semaphore});
        }

        m_render_target = m_device.createTexture("Render_Target_Color", swapchain_data.size,
                                                 TextureFormat::R32G32B32A32_SFLOAT,
                                                 TextureUsageFlags::COLOR_ATTACHMENT | TextureUsageFlags::TRANSFER_SRC |
                                                 TextureUsageFlags::TRANSFER_DST | TextureUsageFlags::STORAGE);

        // TODO: Example render loop. Move it to a separate class.
        // m_descriptor_allocator = m_device.createDescriptorAllocator("Descriptor_Allocator",
        //                                                             {ShaderBindingTypeRatios{ShaderBindingType::STORAGE_IMAGE}});
        // m_layout = m_device.createDescriptorLayout("Descriptor_Layout_Gradient", ShaderStageFlags::COMPUTE,
        //                                            {ShaderBinding{0, ShaderBindingType::STORAGE_IMAGE}});
        // m_set = m_descriptor_allocator.allocate("Descriptor_Set_Gradient", m_layout,
        //                                         {ShaderBindingResource{0, &m_render_target}});
        // m_pipeline_layout = m_device.createPipelineLayout("Pipeline_Layout_Gradient", {m_layout});
        // m_shader = m_device.createShader("Shader_Gradient", "shaders/gradient.spv", ShaderStageFlags::COMPUTE);
        // m_pipeline = m_device.createComputePipeline("Pipeline_Gradient", m_pipeline_layout, m_shader);

        m_data_fence = m_device.createFence("Fence_Data");

        m_data_fence.reset();
        m_data_encoder = m_device.createCommandEncoder("Encoder_Data", m_device.getTransferQueue());
        m_data_encoder.begin();

        auto data_size = m_indices.size() * sizeof(uint32_t);
        m_buffer_indices = m_device.createBuffer(m_data_encoder, "Buffer_Indices", data_size,
                                                 reinterpret_cast<const uint8_t *>(m_indices.data()),
                                                 BufferUsageFlags::INDEX_BUFFER | BufferUsageFlags::STORAGE_BUFFER);

        data_size = m_positions.size() * sizeof(float) * 4;
        m_buffer_positions = m_device.createBuffer(m_data_encoder, "Buffer_Positions", data_size,
                                                   reinterpret_cast<const uint8_t *>(m_positions.data()),
                                                   BufferUsageFlags::VERTEX_BUFFER | BufferUsageFlags::STORAGE_BUFFER);

        data_size = m_colors.size() * sizeof(float) * 4;
        m_buffer_colors = m_device.createBuffer(m_data_encoder, "Buffer_Colors", data_size,
                                                reinterpret_cast<const uint8_t *>(m_colors.data()),
                                                BufferUsageFlags::VERTEX_BUFFER | BufferUsageFlags::STORAGE_BUFFER);

        m_device.getTransferQueue().submit(m_data_encoder.finish(), m_data_fence);
        if (!m_data_fence.wait(FENCE_WAIT_TIMEOUT))
        {
        }
        if (m_device.tryReleaseTemporaryResources(m_data_fence))
        {
            core::Logger::info(LOG_CHANNEL_VULKAN, "Released temporary resources.");
        }

        m_descriptor_allocator = m_device.createDescriptorAllocator("Descriptor_Allocator",
                                                                    {ShaderBindingTypeRatios{ShaderBindingType::STORAGE_BUFFER}});
        m_layout = m_device.createDescriptorLayout("Descriptor_Layout_Basic",
                                                   ShaderStageFlags::VERTEX | ShaderStageFlags::COMPUTE,
                                                   {ShaderBinding{0, ShaderBindingType::STORAGE_BUFFER},
                                                    ShaderBinding{1, ShaderBindingType::STORAGE_BUFFER}});
        m_set = m_descriptor_allocator.allocate("Descriptor_Set_Gradient", m_layout,
                                                {ShaderBindingResource{0, &m_buffer_positions},
                                                 ShaderBindingResource{1, &m_buffer_colors}});
        m_pipeline_layout = m_device.createPipelineLayout("Pipeline_Layout_Basic", {m_layout});
        m_shader = m_device.createShader("Shader_Basic_Vertex", "shaders/basic.spv",
                                         {ShaderStageFlags::VERTEX, ShaderStageFlags::FRAGMENT}, {"mainVS", "mainFS"});

        auto render_state = RenderState{};
        render_state.color_attachments = {ColorAttachment{m_render_target.getFormat(), ColorBlendState::replace()}};
        m_pipeline = m_device.createRenderPipeline("Pipeline_Basic", m_pipeline_layout, {m_shader},
                                                   render_state);

        m_current_index = 0;
    }
    return status;
}

void Renderer::update()
{
}

void Renderer::render()
{
    const auto &[fence, encoder, swapchain_semaphore, render_semaphore] = m_ctxs[m_current_index];

    if (!fence.wait(FENCE_WAIT_TIMEOUT))
    {
        return;
    }
    fence.reset();

    Queue &queue = m_device.getGraphicsQueue();
    auto &swapchain_texture = m_swapchain.getTexture(swapchain_semaphore);

    if (swapchain_semaphore.isValid())
    {
        queue.addWaitSemaphore(swapchain_semaphore);
    }
    queue.addSignalSemaphore(render_semaphore);

    encoder.begin();

    encoder.transitionTextureLayout(m_render_target, TextureLayout::GENERAL);
    encoder.clearTexture(m_render_target, {0.0f, 0.0f, 0.0f, 1.0f});
    encoder.transitionTextureLayout(m_render_target, TextureLayout::COLOR_ATTACHMENT_OPTIMAL);

    {
        encoder.beginRendering({m_render_target}, {}, "Hello Triangle");
        encoder.bindRenderPipeline(m_pipeline);
        encoder.bindDescriptorSet(m_pipeline_layout, 0, m_set);
        encoder.bindIndexBuffer(m_buffer_indices);

        encoder.setViewport(Rect2D{Offset2D{}, m_render_target.getSize()});
        encoder.setScissor(Rect2D{Offset2D{}, m_render_target.getSize()});
        encoder.drawIndexed(3, 1);
        encoder.endRendering();
    }
    // const auto [width, height] = m_render_target.getSize();
    // const std::array<uint32_t, 3> group_count = {
    //     static_cast<uint32_t>(std::ceil(static_cast<float>(width) / 16.0f)),
    //     static_cast<uint32_t>(std::ceil(static_cast<float>(height) / 16.0f)),
    //     1};
    // encoder.dispatch(group_count);

    encoder.transitionTextureLayout(m_render_target, TextureLayout::TRANSFER_SRC_OPTIMAL);
    encoder.transitionTextureLayout(swapchain_texture, TextureLayout::TRANSFER_DST_OPTIMAL);
    encoder.blitTexture(m_render_target, swapchain_texture);
    encoder.transitionTextureLayout(swapchain_texture, TextureLayout::PRESENT_SRC);

    queue.submit(encoder.finish(), fence);

    queue.addWaitSemaphore(render_semaphore);
    queue.present(m_swapchain.present());

    m_current_index = (m_current_index + 1) % m_ctxs.size();
}

void Renderer::lateUpdate()
{

}

void Renderer::clean()
{
    if (!m_device.isValid())
    {
        return;
    }
    m_device.waitIdle();

    m_pipeline.destroy();
    m_shader.destroy();
    m_pipeline_layout.destroy();
    m_layout.destroy();
    m_descriptor_allocator.destroy();

    m_buffer_colors.destroy();
    m_buffer_positions.destroy();
    m_buffer_indices.destroy();
    m_data_encoder.destroy();
    m_data_fence.destroy();

    m_render_target.destroy();
    if (!m_ctxs.empty())
    {
        for (auto &[fence, encoder, swapchain_semaphore, render_semaphore] : m_ctxs)
        {
            swapchain_semaphore.destroy();
            render_semaphore.destroy();
            encoder.destroy();
            fence.destroy();
        }
        m_ctxs.clear();
    }
    m_swapchain.destroy();
    m_device.destroy();
}
}