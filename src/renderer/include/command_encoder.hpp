// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_COMMAND_ENCODER_HPP
#define KIRANA_RENDERER_COMMAND_ENCODER_HPP

#include <array>

#include "texture.hpp"

namespace kirana::renderer
{
struct CommandSubmitInfo
{
    vk::CommandBufferSubmitInfo info{};
};

class CommandEncoder
{
public:
    CommandEncoder() = default;
    ~CommandEncoder() = default;

    bool init(const Device &device, QueueFamilyFlags supported_queue_type = QueueFamilyFlags::GRAPHICS);
    void begin() const;
    CommandSubmitInfo finish() const;
    void destroy();

    /**
     * Transitions the texture layout from current to the given one. The texture is updated with the new value, but it
     * does not take effect until the command encoder is submitted to the queue.
     * @param texture Texture whose layout needs to be transitioned. Texture will be updated with the new layout value.
     * @param new_layout Target layout of the texture.
     */
    void transitionTextureLayout(Texture &texture, TextureLayout new_layout) const;
    /**
     * Clears the texture with the given color / depth / stencil values.
     * @param texture The texture to be cleared.
     * @param color [r, g, b, a] color value in case of color texture and [depth, -, -, -] value in case of depth
     * texture. Values should be in the range of 0.0-1.0.
     * @param stencil Optional stencil value in case of depth-stencil texture.
     */
    void clearTexture(const Texture &texture, const std::array<float, 4> &color, uint32_t stencil = 0) const;

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