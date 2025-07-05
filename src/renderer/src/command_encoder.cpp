// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "command_encoder.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool CommandEncoder::init(const vk::Device device, const uint32_t queue_family)
{
    m_device = device;
    const auto pool_create_info = vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                            queue_family};
    m_pool = m_device.createCommandPool(pool_create_info);

    const auto buffer_create_info = vk::CommandBufferAllocateInfo{m_pool, vk::CommandBufferLevel::ePrimary, 1};
    m_buffer = m_device.allocateCommandBuffers(buffer_create_info).front();
    return true;
}

void CommandEncoder::begin() const
{
    m_buffer.reset();
    m_buffer.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
}

CommandSubmitInfo CommandEncoder::finish() const
{
    m_buffer.end();
    return CommandSubmitInfo{vk::CommandBufferSubmitInfo{m_buffer, 0}};
}

void CommandEncoder::destroy()
{
    if (m_device)
    {
        if (m_pool)
        {
            m_device.destroyCommandPool(m_pool);
            m_pool = nullptr;
            m_buffer = nullptr;
        }
    }
}

void CommandEncoder::transitionTextureLayout(Texture &texture, const TextureLayout new_layout) const
{
    const vk::ImageLayout src_layout = getImageLayout(texture.getLayout());
    const vk::ImageLayout target_layout = getImageLayout(new_layout);
    const vk::ImageAspectFlags image_aspect = isDepthTextureFormat(texture.getFormat())
                                                  ? vk::ImageAspectFlagBits::eDepth
                                                  : vk::ImageAspectFlagBits::eColor;
    const auto image_barrier = vk::ImageMemoryBarrier2{
        vk::PipelineStageFlagBits2::eAllCommands, vk::AccessFlagBits2::eMemoryWrite,
        vk::PipelineStageFlagBits2::eAllCommands, vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
        src_layout, target_layout, {}, {}, texture.getNativeHandle(),
        vk::ImageSubresourceRange{image_aspect, 0, vk::RemainingMipLevels, 0,
                                  vk::RemainingArrayLayers}};

    auto dep_info = vk::DependencyInfo{};
    dep_info.setImageMemoryBarriers({image_barrier});
    m_buffer.pipelineBarrier2(dep_info);

    texture.m_layout = new_layout;
}

void CommandEncoder::clearTexture(const Texture &texture, const std::array<float, 4> &color,
                                  const uint32_t stencil) const
{
    if (isDepthTextureFormat(texture.getFormat()))
    {
        m_buffer.clearDepthStencilImage(texture.getNativeHandle(), getImageLayout(texture.getLayout()),
                                        vk::ClearDepthStencilValue{color[0], stencil},
                                        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eDepth, 0,
                                                                  vk::RemainingMipLevels,
                                                                  0, vk::RemainingArrayLayers});

    }
    else
    {
        m_buffer.clearColorImage(texture.getNativeHandle(), getImageLayout(texture.getLayout()),
                                 vk::ClearColorValue{color},
                                 vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, vk::RemainingMipLevels,
                                                           0, vk::RemainingArrayLayers});
    }
}


}