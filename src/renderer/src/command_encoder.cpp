// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "command_encoder.hpp"

#include "helpers_vulkan.hpp"
#include "buffer.hpp"
#include "texture.hpp"
#include "pipeline_layout.hpp"
#include "pipeline_compute.hpp"
#include "pipeline_render.hpp"
#include "descriptor_set.hpp"

namespace kirana::renderer
{
bool CommandEncoder::init(const vk::Device device, const std::string &name, const uint32_t queue_family)
{
    m_device = device;
    m_name = name;
    const auto pool_create_info = vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                            queue_family};
    m_pool = m_device.createCommandPool(pool_create_info);

    const auto buffer_create_info = vk::CommandBufferAllocateInfo{m_pool, vk::CommandBufferLevel::ePrimary, 1};
    m_buffer = m_device.allocateCommandBuffers(buffer_create_info).front();

#ifdef DEBUG
    if (!m_name.empty())
    {

        auto new_name = "Command_Pool_" + m_name;
        auto handle = reinterpret_cast<uint64_t>(static_cast<VkCommandPool>(m_pool));
        setDebugName(m_device, vk::ObjectType::eCommandPool, handle, new_name);

        new_name = "Command_Buffer_" + m_name;
        handle = reinterpret_cast<uint64_t>(static_cast<VkCommandBuffer>(m_buffer));
        setDebugName(m_device, vk::ObjectType::eCommandBuffer, handle, new_name);
    }
#endif
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

void CommandEncoder::addBufferBarrier(const Buffer &buffer, const MemoryAccessFlags src_access,
                                      const MemoryAccessFlags dst_access, const PipelineStageFlags src_stage,
                                      const PipelineStageFlags dst_stage) const
{
    const auto barrier = vk::BufferMemoryBarrier2{getPipelineStageFlags(src_stage), getAccessFlags(src_access),
                                                  getPipelineStageFlags(dst_stage), getAccessFlags(dst_access),
                                                  vk::QueueFamilyIgnored, vk::QueueFamilyIgnored,
                                                  buffer.getNativeHandle(), 0, vk::WholeSize};
    vk::DependencyInfo dep_info = {};
    dep_info.setBufferMemoryBarriers({barrier});
    m_buffer.pipelineBarrier2(dep_info);
}

void CommandEncoder::copyBuffer(const Buffer &src, const Buffer &dst,
                                const std::vector<BufferCopyRegion> &regions) const
{
    std::vector<vk::BufferCopy2> vk_regions = {};
    if (regions.empty())
    {
        vk_regions.emplace_back(0, 0, src.getSize());
    }
    else
    {
        for (const auto &r : regions)
        {
            vk_regions.emplace_back(r.src_offset, r.dst_offset, r.size);
        }
    }
    m_buffer.copyBuffer2(vk::CopyBufferInfo2{src.getNativeHandle(), dst.getNativeHandle(), vk_regions});
}

void CommandEncoder::copyTexture(const Texture &src, const Texture &dst,
                                 const std::vector<TextureCopyRegion> &regions) const
{
    const vk::ImageAspectFlags src_aspect = isDepthTextureFormat(src.getFormat())
                                                ? vk::ImageAspectFlagBits::eDepth
                                                : vk::ImageAspectFlagBits::eColor;
    const vk::ImageAspectFlags dst_aspect = isDepthTextureFormat(dst.getFormat())
                                                ? vk::ImageAspectFlagBits::eDepth
                                                : vk::ImageAspectFlagBits::eColor;
    const auto src_subresource = vk::ImageSubresourceLayers{src_aspect, 0, 0, 1};
    const auto dst_subresource = vk::ImageSubresourceLayers{dst_aspect, 0, 0, 1};

    std::vector<vk::ImageCopy2> vk_regions = {};
    if (regions.empty())
    {
        vk_regions.emplace_back(src_subresource, vk::Offset3D{0, 0, 0}, dst_subresource,
                                vk::Offset3D{0, 0, 0}, getExtent3D(src.getSize()));
    }
    else
    {
        for (const auto &r : regions)
        {
            vk_regions.emplace_back(src_subresource, getOffset3D(r.src_offset), dst_subresource,
                                    getOffset3D(r.dst_offset), getExtent3D(r.size));
        }
    }
    m_buffer.copyImage2(vk::CopyImageInfo2{src.getNativeHandle(), getImageLayout(src.getLayout()),
                                           dst.getNativeHandle(), getImageLayout(dst.getLayout()), vk_regions});
}

void CommandEncoder::addTextureBarrier(Texture &texture, const TextureLayout new_layout,
                                       const MemoryAccessFlags src_access, const MemoryAccessFlags dst_access,
                                       const PipelineStageFlags src_stage, const PipelineStageFlags dst_stage) const
{
    const vk::ImageAspectFlags image_aspect = isDepthTextureFormat(texture.getFormat())
                                                  ? vk::ImageAspectFlagBits::eDepth
                                                  : vk::ImageAspectFlagBits::eColor;
    const auto subresource = vk::ImageSubresourceRange{image_aspect, 0, vk::RemainingMipLevels, 0,
                                                       vk::RemainingArrayLayers};
    const auto barrier = vk::ImageMemoryBarrier2{getPipelineStageFlags(src_stage), getAccessFlags(src_access),
                                                 getPipelineStageFlags(dst_stage), getAccessFlags(dst_access),
                                                 getImageLayout(texture.getLayout()), getImageLayout(new_layout),
                                                 vk::QueueFamilyIgnored, vk::QueueFamilyIgnored,
                                                 texture.getNativeHandle(), subresource};
    vk::DependencyInfo dep_info = {};
    dep_info.setImageMemoryBarriers({barrier});
    m_buffer.pipelineBarrier2(dep_info);
    texture.m_layout = new_layout;
}


void CommandEncoder::transitionTextureLayout(Texture &texture, const TextureLayout new_layout) const
{
    transitionImageLayout(m_buffer, texture.getNativeHandle(), getFormat(texture.getFormat()),
                          getImageLayout(texture.getLayout()), getImageLayout(new_layout));
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


void CommandEncoder::blitTexture(const Texture &src, const Texture &dst, Rect2D src_region, Rect2D dst_region) const
{
    const auto src_aspect = isDepthTextureFormat(src.getFormat())
                                ? vk::ImageAspectFlagBits::eDepth
                                : vk::ImageAspectFlagBits::eColor;
    if (!src_region.isValid())
    {
        const auto [width, height] = src.getSize();
        src_region = Rect2D{0, 0, width, height};
    }

    const auto dst_aspect = isDepthTextureFormat(dst.getFormat())
                                ? vk::ImageAspectFlagBits::eDepth
                                : vk::ImageAspectFlagBits::eColor;
    if (!dst_region.isValid())
    {
        const auto [width, height] = dst.getSize();
        dst_region = Rect2D{0, 0, width, height};
    }

    const auto blit_image = vk::ImageBlit2{
        vk::ImageSubresourceLayers{src_aspect, 0, 0, 1}, getOffset3DFromRect(src_region),
        vk::ImageSubresourceLayers{dst_aspect, 0, 0, 1}, getOffset3DFromRect(dst_region)};
    const auto blit_info = vk::BlitImageInfo2{src.getNativeHandle(), getImageLayout(src.getLayout()),
                                              dst.getNativeHandle(), getImageLayout(dst.getLayout()),
                                              {blit_image}};
    m_buffer.blitImage2(blit_info);
}

void CommandEncoder::beginRendering(const std::vector<Texture> &color_attachments, const Texture &depth_attachment,
                                    const std::string &name, const std::array<float, 4> &debug_color) const
{
#ifdef DEBUG
    if (!name.empty())
    {
        m_has_render_label = true;
        m_buffer.beginDebugUtilsLabelEXT(vk::DebugUtilsLabelEXT{name.c_str(), debug_color});
    }
#endif
    if (color_attachments.empty() && !depth_attachment.isValid())
    {
        return;
    }
    const auto extent = !color_attachments.empty() ? color_attachments[0].getSize() : depth_attachment.getSize();
    const auto render_area = vk::Rect2D{vk::Offset2D{0, 0}, getExtent2D(extent)};

    std::vector<vk::RenderingAttachmentInfo> color_attachments_info = {};
    color_attachments_info.reserve(color_attachments.size());
    for (const auto &c : color_attachments)
    {
        color_attachments_info.emplace_back(c.getNativeViewHandle(), getImageLayout(c.getLayout()));
    }
    auto render_info = vk::RenderingInfo{vk::RenderingFlags{}, render_area, 1};
    render_info.setColorAttachments(color_attachments_info);

    auto depth_attachment_info = vk::RenderingAttachmentInfo{};
    if (depth_attachment.isValid())
    {
        depth_attachment_info = vk::RenderingAttachmentInfo{depth_attachment.getNativeViewHandle(),
                                                            getImageLayout(depth_attachment.getLayout())};
        render_info.setPDepthAttachment(&depth_attachment_info);
    }
    m_buffer.beginRendering(render_info);
}

void CommandEncoder::bindComputePipeline(const PipelineCompute &pipeline) const
{
    m_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, pipeline.getNativeHandle());
    m_current_pipeline_bind_point = vk::PipelineBindPoint::eCompute;
}

void CommandEncoder::bindRenderPipeline(const PipelineRender &pipeline) const
{
    m_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.getNativeHandle());
    m_current_pipeline_bind_point = vk::PipelineBindPoint::eGraphics;
}

void CommandEncoder::bindDescriptorSet(const PipelineLayout &layout, const uint32_t index, const DescriptorSet &set,
                                       const std::vector<uint32_t> &dynamic_offsets) const
{
    m_buffer.bindDescriptorSets(m_current_pipeline_bind_point, layout.getNativeHandle(), index, set.getNativeHandle(),
                                dynamic_offsets);
}

void CommandEncoder::bindIndexBuffer(const Buffer &buffer, const uint64_t offset, const IndexType index_type) const
{
    m_buffer.bindIndexBuffer(buffer.getNativeHandle(), offset, getIndexType(index_type));
}

void CommandEncoder::setViewport(const Rect2D &area, const float min_depth, const float max_depth) const
{
    const auto viewport = vk::Viewport{static_cast<float>(area.offset.x), static_cast<float>(area.offset.y),
                                       static_cast<float>(area.size.width), static_cast<float>(area.size.height),
                                       min_depth, max_depth};
    m_buffer.setViewport(0, {viewport});
}

void CommandEncoder::setScissor(const Rect2D &area) const
{
    m_buffer.setScissor(0, {getRect2D(area)});
}


void CommandEncoder::dispatch(const std::array<uint32_t, 3> &group_count) const
{
    m_buffer.dispatch(group_count[0], group_count[1], group_count[2]);
}

void CommandEncoder::draw(const uint32_t vertex_count, const uint32_t instance_count, const uint32_t first_vertex,
                          const uint32_t first_instance) const
{
    m_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
}

void CommandEncoder::drawIndexed(const uint32_t index_count, const uint32_t instance_count, const uint32_t first_index,
                                 const int32_t vertex_offset,
                                 const uint32_t first_instance) const
{
    m_buffer.drawIndexed(index_count, instance_count, first_index, vertex_offset, first_instance);
}

void CommandEncoder::endRendering() const
{
    m_buffer.endRendering();
#ifdef DEBUG
    if (m_has_render_label)
    {
        m_buffer.endDebugUtilsLabelEXT();
        m_has_render_label = false;
    }
#endif
}
}