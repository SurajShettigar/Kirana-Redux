// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_COMMAND_ENCODER_HPP
#define KIRANA_RENDERER_COMMAND_ENCODER_HPP

#include <array>

#include "texture.hpp"
#include "descriptor_set.hpp"
#include "pipeline_compute.hpp"

namespace kirana::renderer
{
struct CommandSubmitInfo
{
    vk::CommandBufferSubmitInfo info{};
};

class CommandEncoder
{
    friend class Device;

public:
    CommandEncoder() = default;
    ~CommandEncoder() = default;

    void destroy();

    void begin() const;
    [[nodiscard]] CommandSubmitInfo finish() const;
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

    /**
     * Copies texture content from one texture to another.
     * @param src The source texture from which the content will be copied from.
     * @param dst The destination texture onto which the contents will be copied to.
     * @param src_region The region of source texture to copy from. If empty, the entire region is copied.
     * @param dst_region The region of destination texture to copy to. If empty, the entire region is copied.
     */
    void blitTexture(const Texture &src, const Texture &dst, Rect2D src_region = {}, Rect2D dst_region = {}) const;

    void bindComputePipeline(const PipelineCompute &pipeline) const;

    void bindDescriptorSet(const PipelineLayout &layout, uint32_t index, const DescriptorSet &set,
                           const std::vector<uint32_t> &dynamic_offsets = {}) const;

    void dispatch(const std::array<uint32_t, 3> &group_count) const;

    [[nodiscard]] bool isValid() const
    {
        return m_pool != nullptr && m_buffer != nullptr;
    }

private:
    vk::Device m_device{nullptr};
    vk::CommandPool m_pool{nullptr};
    vk::CommandBuffer m_buffer{nullptr};

    mutable vk::PipelineBindPoint m_current_pipeline_bind_point{vk::PipelineBindPoint::eGraphics};

    bool init(vk::Device device, uint32_t queue_family);
};
}

#endif //KIRANA_RENDERER_COMMAND_ENCODER_HPP