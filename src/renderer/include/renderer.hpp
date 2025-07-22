// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_RENDERER_HPP
#define KIRANA_RENDERER_RENDERER_HPP

#include <no_copy.hpp>

#include "common.hpp"
#include "device.hpp"

namespace kirana::renderer
{
struct RenderContext
{
    Fence m_fence{};
    CommandEncoder encoder{};
    Semaphore swapchain_semaphore{};
    Semaphore render_semaphore{};
};

class Renderer : core::NoCopy
{
public:
    Renderer() = default;
    ~Renderer() = default;


    Renderer(const Renderer &renderer) = delete;
    Renderer &operator=(const Renderer &renderer) = delete;

    bool init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data = {});
    void update();
    void render();
    void lateUpdate();
    void clean();

private:
    Device m_device{};
    Swapchain m_swapchain{};
    std::vector<RenderContext> m_ctxs{};
    Texture m_render_target{};

    // TODO: Example render loop. Move it to a separate class.
    // DescriptorAllocator m_descriptor_allocator{};
    // DescriptorLayout m_layout{};
    // DescriptorSet m_set{};
    // PipelineLayout m_pipeline_layout{};
    // Shader m_shader{};
    // PipelineCompute m_pipeline{};

    std::vector<uint32_t> m_indices{0, 1, 2};
    std::vector<std::array<float, 4>> m_positions{{-1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},
                                                  {0.0f, -1.0f, 0.0f, 1.0f}};
    std::vector<std::array<float, 4>> m_colors{{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f},
                                               {0.0f, 0.0f, 1.0f, 1.0f}};
    Buffer m_buffer_indices{};
    Buffer m_buffer_positions{};
    Buffer m_buffer_colors{};
    Fence m_data_fence{};
    CommandEncoder m_data_encoder{};

    DescriptorAllocator m_descriptor_allocator{};
    DescriptorLayout m_layout{};
    DescriptorSet m_set{};
    PipelineLayout m_pipeline_layout{};
    Shader m_shader{};
    PipelineRender m_pipeline{};

    size_t m_current_index{0};
};
}

#endif //KIRANA_RENDERER_RENDERER_HPP