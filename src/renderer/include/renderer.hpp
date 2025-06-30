// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_RENDERER_HPP
#define KIRANA_RENDERER_RENDERER_HPP

#include <no_copy.hpp>

#include "common.hpp"
#include "swapchain.hpp"
#include "command_encoder.hpp"

namespace kirana::renderer
{
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
    std::vector<CommandEncoder> m_cmd_encoders_graphic {};

    size_t m_current_index {0};
};
}

#endif //KIRANA_RENDERER_RENDERER_HPP