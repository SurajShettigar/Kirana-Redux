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

    bool init(const DeviceInitializationData &init_data, const SwapchainData &swapchain_data = {},
              const scene::Scene &scene = {});
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
    TextureHandle m_render_target{};
    TextureHandle m_depth_buffer{};

    SceneData m_scene_data{};

    DescriptorLayout m_layout_env{};
    DescriptorSet m_set_env{};
    PipelineLayout m_pipeline_layout_env{};
    Shader m_shader_env{};
    PipelineRender m_pipeline_env{};

    DescriptorLayout m_layout{};
    DescriptorSet m_set{};
    PipelineLayout m_pipeline_layout{};
    Shader m_shader{};
    PipelineRender m_pipeline{};

    DescriptorLayout m_layout_tonemap{};
    DescriptorSet m_set_tonemap{};
    PipelineLayout m_pipeline_layout_tonemap{};
    Shader m_shader_tonemap{};
    PipelineCompute m_pipeline_tonemap{};

    size_t m_current_index{0};
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_RENDERER_HPP