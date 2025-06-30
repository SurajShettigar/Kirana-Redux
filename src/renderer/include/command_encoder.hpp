// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_COMMAND_ENCODER_HPP
#define KIRANA_RENDERER_COMMAND_ENCODER_HPP

#include "swapchain.hpp"

namespace kirana::renderer
{
class CommandEncoder
{
public:
    CommandEncoder() = default;
    ~CommandEncoder() = default;

    bool init(const Device &device);
    void begin(const SwapchainTexture &render_target);
    void finish();
    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_pool != nullptr && m_buffer != nullptr && m_fence != nullptr;
    }

private:
    vk::Device m_device{nullptr};
    vk::CommandPool m_pool{nullptr};
    vk::CommandBuffer m_buffer{nullptr};
    vk::Fence m_fence{nullptr};
};
}

#endif //KIRANA_RENDERER_COMMAND_ENCODER_HPP