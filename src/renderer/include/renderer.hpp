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
    Fence m_fence{};
    Swapchain m_swapchain{};
    std::vector<RenderContext> m_ctxs{};
    Texture m_render_target {};

    size_t m_current_index{0};
};
}

#endif //KIRANA_RENDERER_RENDERER_HPP