// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

#include "helpers_vulkan.hpp"
#include "logger.hpp"

namespace kirana::renderer
{

inline TextureHandle createRenderTexture(Device &device, const Size2D &size, bool depth = false)
{
    if (depth)
    {
        return device.createTexture("Render_Target_Depth", size, TextureFormat::D32_SFLOAT,
                                    TextureUsageFlags::DEPTH_STENCIL_ATTACHMENT | TextureUsageFlags::TRANSFER_DST,
                                    TextureLayout::GENERAL, nullptr, ResourceMemoryType::DEVICE,
                                    ResourceMemoryFlags::DEDICATED_MEMORY);
    }
    return device.createTexture("Render_Target_Color", size, TextureFormat::R32G32B32A32_SFLOAT,
                                TextureUsageFlags::COLOR_ATTACHMENT | TextureUsageFlags::TRANSFER_SRC |
                                    TextureUsageFlags::TRANSFER_DST | TextureUsageFlags::STORAGE,
                                TextureLayout::GENERAL, nullptr, ResourceMemoryType::DEVICE,
                                ResourceMemoryFlags::DEDICATED_MEMORY);
}

bool Renderer::init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data,
                    const scene::Scene &scene)
{
    bool status = m_device.init(init_data);
    if (status && init_data.surface.isValid())
    {
        m_swapchain = m_device.createSwapchain("Swapchain", swapchain_data);
        status = m_swapchain.isValid();
    }
    if (!status)
    {
        return false;
    }
    const bool has_swapchain = m_swapchain.isValid();
    const auto num_ctxs =
        has_swapchain ? m_swapchain.getTextureCount() : static_cast<size_t>(swapchain_data.buffer_mode);
    m_ctxs.reserve(num_ctxs);
    for (size_t i = 0; i < num_ctxs; ++i)
    {
        const auto fence = m_device.createFence("Fence_Main_" + std::to_string(i));
        const auto encoder =
            m_device.createCommandEncoder("Encoder_Render_" + std::to_string(i), m_device.getGraphicsQueue());
        const auto semaphore = m_device.createSemaphore("Semaphore_Render_" + std::to_string(i));
        m_ctxs.emplace_back(RenderContext{fence, encoder, semaphore});
    }

    m_device.beginAllocation();
    m_render_target = createRenderTexture(m_device, swapchain_data.size, false);
    m_depth_buffer = createRenderTexture(m_device, swapchain_data.size, true);
    m_device.endAllocation();

    m_layout_env = m_device.createDescriptorLayout("Descriptor_Layout_Environment",
                                                   ShaderStageFlags::VERTEX | ShaderStageFlags::COMPUTE |
                                                       ShaderStageFlags::FRAGMENT,
                                                   {
                                                       // Camera Buffer
                                                       ShaderBinding{0, ShaderBindingType::UNIFORM_BUFFER},
                                                       // Environment Data Buffer
                                                       ShaderBinding{1, ShaderBindingType::UNIFORM_BUFFER},
                                                       // Environment Texture
                                                       ShaderBinding{2, ShaderBindingType::COMBINED_IMAGE_SAMPLER},
                                                   });
    m_pipeline_layout_env = m_device.createPipelineLayout("Pipeline_Layout_Environment", {m_layout_env});
    m_shader_env = m_device.createShader("Shader_Environment", "shaders/environment.spv",
                                         {ShaderStageFlags::VERTEX, ShaderStageFlags::FRAGMENT}, {"mainVS", "mainFS"});

    m_layout = m_device.createDescriptorLayout(
        "Descriptor_Layout_Basic", ShaderStageFlags::VERTEX | ShaderStageFlags::COMPUTE | ShaderStageFlags::FRAGMENT,
        {
            // Camera Buffer
            ShaderBinding{0, ShaderBindingType::UNIFORM_BUFFER},
            // Position Buffer
            ShaderBinding{1, ShaderBindingType::STORAGE_BUFFER},
            // Normal Buffer
            ShaderBinding{2, ShaderBindingType::STORAGE_BUFFER},
            // UV Buffer
            ShaderBinding{3, ShaderBindingType::STORAGE_BUFFER},
            // Mesh Buffer
            ShaderBinding{4, ShaderBindingType::STORAGE_BUFFER},
            // Transform Buffer
            ShaderBinding{5, ShaderBindingType::STORAGE_BUFFER},
            // Mesh Instance Buffer
            ShaderBinding{6, ShaderBindingType::STORAGE_BUFFER},
            // Texture Data Buffer
            ShaderBinding{7, ShaderBindingType::STORAGE_BUFFER},
            // Material Data Buffer
            ShaderBinding{8, ShaderBindingType::STORAGE_BUFFER},
            // Bindless Samplers
            ShaderBinding{9, ShaderBindingType::SAMPLER, ShaderStageFlags::NONE, 1024, true},
            // Bindless Textures
            ShaderBinding{10, ShaderBindingType::SAMPLED_IMAGE, ShaderStageFlags::NONE, 16384, true},
        });
    m_pipeline_layout = m_device.createPipelineLayout("Pipeline_Layout_Basic", {m_layout});
    m_shader = m_device.createShader("Shader_Basic_Vertex", "shaders/basic.spv",
                                     {ShaderStageFlags::VERTEX, ShaderStageFlags::FRAGMENT}, {"mainVS", "mainFS"});

    m_layout_tonemap = m_device.createDescriptorLayout("Descriptor_Layout_Tonemap", ShaderStageFlags::COMPUTE,
                                                       {
                                                           // Render Texture
                                                           ShaderBinding{0, ShaderBindingType::STORAGE_IMAGE},
                                                       });
    m_pipeline_layout_tonemap = m_device.createPipelineLayout("Pipeline_Layout_Tonemap", {m_layout_tonemap});
    m_shader_tonemap = m_device.createShader("Shader_Tonemap", "shaders/tonemapping.spv", {ShaderStageFlags::COMPUTE});

    auto render_state = RenderState{};
    render_state.color_attachments = {
        ColorAttachment{m_device.getTexture(m_render_target)->getFormat(), ColorBlendState::replace()}};
    render_state.depth_stencil_attachment =
        DepthStencilAttachment{m_device.getTexture(m_depth_buffer)->getFormat(), DepthStencilState{}};

    render_state.rasterization.cull_mode = CullMode::FRONT;
    m_pipeline_env =
        m_device.createRenderPipeline("Pipeline_Environment", m_pipeline_layout_env, {m_shader_env}, render_state);

    render_state.rasterization.cull_mode = CullMode::BACK;
    m_pipeline = m_device.createRenderPipeline("Pipeline_Basic", m_pipeline_layout, {m_shader}, render_state);

    m_pipeline_tonemap =
        m_device.createComputePipeline("Pipeline_Tonemap", m_pipeline_layout_tonemap, m_shader_tonemap);
    m_set_tonemap = m_device.allocateDescriptorSet("Descriptor_Set_Tonemap", m_layout_tonemap,
                                                   {
                                                       ShaderBindingResource{0, m_render_target},
                                                   });

    loadScene(scene);

    m_current_index = 0;
    return status;
}

void Renderer::update()
{
}

void Renderer::render()
{
    const auto &fence = m_ctxs[m_current_index].fence;
    const auto &encoder = m_ctxs[m_current_index].encoder;
    Queue &queue = m_device.getGraphicsQueue();

    if (!fence.wait(FENCE_WAIT_TIMEOUT))
    {
        return;
    }

    const auto &swapchain_result = m_swapchain.getTexture();
    if (!swapchain_result)
    {
        return;
    }
    const auto [swapchain_index, swapchain_texture, swapchain_semaphore] = swapchain_result.value();

    fence.reset();

    // TODO: Add better texture resizing.
    if (swapchain_texture.getSize() != m_device.getTexture(m_render_target)->getSize())
    {
        const auto size = swapchain_texture.getSize();
        m_device.destroyTexture(m_render_target);
        m_device.destroyTexture(m_depth_buffer);

        m_device.beginAllocation();
        m_render_target = createRenderTexture(m_device, size, false);
        m_depth_buffer = createRenderTexture(m_device, size, true);
        m_device.endAllocation();

        m_set_tonemap.updateBindingResources(m_layout_tonemap, {ShaderBindingResource{0, m_render_target}});
    }

    const auto &render_semaphore = m_ctxs[swapchain_index].semaphore;

    queue.addWaitSemaphore(swapchain_semaphore);
    queue.addSignalSemaphore(render_semaphore);

    encoder.begin();

    const Texture &render_target = *m_device.getTexture(m_render_target);
    const Texture &depth_buffer = *m_device.getTexture(m_depth_buffer);
    const Size2D render_size = render_target.getSize();

    encoder.transitionTextureLayout(render_target, TextureLayout::GENERAL);
    encoder.transitionTextureLayout(depth_buffer, TextureLayout::GENERAL);
    encoder.clearTexture(render_target, {0.0f, 0.0f, 0.0f, 1.0f});
    encoder.clearTexture(depth_buffer, {1.0f, 0.0f, 0.0f, 0.0f});
    if (m_scene_data.isValid())
    {
        encoder.transitionTextureLayout(render_target, TextureLayout::COLOR_ATTACHMENT_OPTIMAL);
        encoder.transitionTextureLayout(depth_buffer, TextureLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        encoder.beginRendering({render_target}, {depth_buffer}, "Environment");
        encoder.bindRenderPipeline(m_pipeline_env);
        encoder.bindDescriptorSet(m_pipeline_layout_env, 0, m_set_env);
        encoder.setViewport(Rect2D{Offset2D{}, render_size});
        encoder.setScissor(Rect2D{Offset2D{}, render_size});
        encoder.draw(3, 1);
        encoder.endRendering();

        encoder.beginRendering({render_target}, {depth_buffer}, "Basic");
        encoder.bindRenderPipeline(m_pipeline);
        encoder.bindDescriptorSet(m_pipeline_layout, 0, m_set);

        encoder.setViewport(Rect2D{Offset2D{}, render_size});
        encoder.setScissor(Rect2D{Offset2D{}, render_size});

        m_scene_data.forEachRenderable([&](const MeshData &mesh, const MeshInstancesData &instances) {
            if (mesh.index_format == scene::IndexFormat::UINT_8)
            {
                encoder.bindIndexBuffer(*m_device.getBuffer(m_scene_data.getIndexBuffer8()), 0, IndexType::UINT8);
            }
            else if (mesh.index_format == scene::IndexFormat::UINT_16)
            {
                encoder.bindIndexBuffer(*m_device.getBuffer(m_scene_data.getIndexBuffer16()), 0, IndexType::UINT16);
            }
            else
            {
                encoder.bindIndexBuffer(*m_device.getBuffer(m_scene_data.getIndexBuffer32()));
            }
            encoder.drawIndexed(mesh.index_count, instances.instance_count, mesh.index_offset, 0,
                                instances.instance_offset);
        });
        encoder.endRendering();
    }

    encoder.transitionTextureLayout(render_target, TextureLayout::GENERAL);
    encoder.bindComputePipeline(m_pipeline_tonemap);
    encoder.bindDescriptorSet(m_pipeline_layout_tonemap, 0, m_set_tonemap);
    const auto [width, height] = render_size;
    const std::array<uint32_t, 3> group_count = {static_cast<uint32_t>(std::ceil(static_cast<float>(width) / 16.0f)),
                                                 static_cast<uint32_t>(std::ceil(static_cast<float>(height) / 16.0f)),
                                                 1};
    encoder.dispatch(group_count);

    encoder.transitionTextureLayout(render_target, TextureLayout::TRANSFER_SRC_OPTIMAL);
    encoder.transitionTextureLayout(swapchain_texture, TextureLayout::TRANSFER_DST_OPTIMAL);
    encoder.blitTexture(render_target, swapchain_texture);
    encoder.transitionTextureLayout(swapchain_texture, TextureLayout::PRESENT_SRC);

    queue.submit(encoder.finish(), fence);

    queue.addWaitSemaphore(render_semaphore);
    queue.present(m_swapchain);

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

    m_scene_data.destroy(m_device);

    m_pipeline_tonemap.destroy();
    m_shader_tonemap.destroy();
    m_pipeline_layout_tonemap.destroy();
    m_layout_tonemap.destroy();

    m_pipeline.destroy();
    m_shader.destroy();
    m_pipeline_layout.destroy();
    m_layout.destroy();

    m_pipeline_env.destroy();
    m_shader_env.destroy();
    m_pipeline_layout_env.destroy();
    m_layout_env.destroy();

    m_device.destroyTexture(m_depth_buffer);
    m_device.destroyTexture(m_render_target);
    if (!m_ctxs.empty())
    {
        for (auto &[fence, encoder, semaphore] : m_ctxs)
        {
            semaphore.destroy();
            encoder.destroy();
            fence.destroy();
        }
        m_ctxs.clear();
    }
    m_swapchain.destroy();
    m_device.destroy();
}

void Renderer::resize(const Size2D &size)
{
    m_swapchain.resize(size);
}

bool Renderer::loadScene(const scene::Scene &scene)
{
    m_scene_data.init(m_device, scene);
    if (m_scene_data.isValid())
    {
        m_set_env =
            m_device.allocateDescriptorSet("Descriptor_Set_Environment", m_layout_env,
                                           {
                                               ShaderBindingResource{0, m_scene_data.getCameraBuffer()},
                                               ShaderBindingResource{1, m_scene_data.getEnvironmentLightBuffer()},
                                               ShaderBindingResource{2, m_scene_data.getEnvironmentLightTexture(),
                                                                     m_scene_data.getEnvironmentLightTextureSampler()},
                                           });

        m_set = m_device.allocateDescriptorSet("Descriptor_Set_Basic", m_layout,
                                               {ShaderBindingResource{0, m_scene_data.getCameraBuffer()},
                                                ShaderBindingResource{1, m_scene_data.getPositionBuffer()},
                                                ShaderBindingResource{2, m_scene_data.getNormalBuffer()},
                                                ShaderBindingResource{3, m_scene_data.getUVBuffer()},
                                                ShaderBindingResource{4, m_scene_data.getMeshesBuffer()},
                                                ShaderBindingResource{5, m_scene_data.getTransformsBuffer()},
                                                ShaderBindingResource{6, m_scene_data.getMeshInstancesBuffer()},
                                                ShaderBindingResource{7, m_scene_data.getTextureDataBuffer()},
                                                ShaderBindingResource{8, m_scene_data.getMaterialsBuffer()},
                                                ShaderBindingResource{9, m_scene_data.getTextureSamplers()},
                                                ShaderBindingResource{10, m_scene_data.getTextures()}});
        return true;
    }
    return false;
}

bool Renderer::updateCamera(const scene::Matrix4 &view_matrix, const scene::Matrix4 &projection_matrix)
{
    return m_scene_data.updateCamera(m_device, view_matrix, projection_matrix);
}
} // namespace kirana::renderer