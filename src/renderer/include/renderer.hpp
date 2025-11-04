// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_RENDERER_HPP
#define KIRANA_RENDERER_RENDERER_HPP

#include "common.hpp"
#include "device.hpp"
#include "scene_data.hpp"

#include <no_copy.hpp>

namespace kirana::renderer
{
struct RenderContext
{
    Fence fence{};
    CommandEncoder encoder{};
    Semaphore semaphore{};
};

class Renderer : core::NoCopy
{
public:
    Renderer() = default;
    ~Renderer() = default;


    Renderer(const Renderer &renderer) = delete;
    Renderer &operator=(const Renderer &renderer) = delete;

    bool init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data = {}, const scene::Scene &scene = {});
    void update();
    void render();
    void lateUpdate();
    void clean();

    void resize(const Size2D &size);

    bool loadScene(const scene::Scene &scene);
    bool updateCamera(const scene::Matrix4 &view_matrix, const scene::Matrix4 &projection_matrix);
private:
    Device m_device{};
    Swapchain m_swapchain{};
    std::vector<RenderContext> m_ctxs{};
    Texture m_render_target{};
    Texture m_depth_buffer{};

    SceneData m_scene_data{};

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