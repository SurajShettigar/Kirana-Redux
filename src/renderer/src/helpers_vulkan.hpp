// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_HELPERS_VULKAN_HPP
#define KIRANA_RENDERER_HELPERS_VULKAN_HPP

#include <vulkan/vulkan.hpp>

#include "common.hpp"

namespace kirana::renderer
{
const std::string LOG_CHANNEL_VULKAN = "RENDERER_VULKAN";

/// Timeout in nanoseconds.
constexpr uint32_t FENCE_WAIT_TIMEOUT = 1000000000; // 1 second
constexpr uint32_t SWAPCHAIN_FETCH_TIMEOUT = 1000000000; // 1 second

inline void setDebugName(const vk::Device device, const vk::ObjectType type, const uint64_t handle,
                         const std::string &name)
{
#ifdef DEBUG
    device.setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT{type, handle, name.c_str()});
#endif
}

constexpr vk::IndexType getIndexType(const IndexType type)
{
    switch (type)
    {
    case IndexType::UINT8:
        return vk::IndexType::eUint8;
    case IndexType::UINT16:
        return vk::IndexType::eUint16;
    case IndexType::UINT32:
        return vk::IndexType::eUint32;
    case IndexType::NONE:
        return vk::IndexType::eNoneKHR;
    default:
        return vk::IndexType::eUint32;
    }
}

constexpr vk::Extent2D getExtent2D(const Size2D &size)
{
    return vk::Extent2D{size.width, size.height};
}

constexpr vk::Extent3D getExtent3D(const Size2D &size)
{
    return vk::Extent3D{size.width, size.height, 1};
}

constexpr vk::Extent3D getExtent3D(const Size3D &size)
{
    return vk::Extent3D{size.width, size.height, size.depth};
}

constexpr vk::Offset2D getOffset2D(const Offset2D &offset)
{
    return vk::Offset2D{offset.x, offset.y};
}

constexpr vk::Offset3D getOffset3D(const Offset2D &offset)
{
    return vk::Offset3D{offset.x, offset.y, 0};
}

constexpr vk::Offset3D getOffset3D(const Offset3D &offset)
{
    return vk::Offset3D{offset.x, offset.y, offset.z};
}

constexpr std::array<vk::Offset2D, 2> getOffset2DFromRect(const Rect2D &rect)
{
    return {getOffset2D(rect.offset),
            vk::Offset2D{rect.offset.x + static_cast<int32_t>(rect.size.width),
                         rect.offset.y + static_cast<int32_t>(rect.size.height)}};
}

constexpr std::array<vk::Offset3D, 2> getOffset3DFromRect(const Rect2D &rect)
{
    return {getOffset3D(rect.offset),
            vk::Offset3D{rect.offset.x + static_cast<int32_t>(rect.size.width),
                         rect.offset.y + static_cast<int32_t>(rect.size.height), 1}};
}

constexpr std::array<vk::Offset3D, 2> getOffset3DFromRect(const Rect3D &rect)
{
    return {getOffset3D(rect.offset),
            vk::Offset3D{rect.offset.x + static_cast<int32_t>(rect.size.width),
                         rect.offset.y + static_cast<int32_t>(rect.size.height),
                         rect.offset.z + static_cast<int32_t>(rect.size.depth)}};
}

constexpr vk::Rect2D getRect2D(const Rect2D &rect)
{
    return vk::Rect2D{getOffset2D(rect.offset), getExtent2D(rect.size)};
}

constexpr vk::BufferUsageFlags getBufferUsageFlags(const BufferUsageFlags flags)
{
    return static_cast<vk::BufferUsageFlags>(static_cast<uint32_t>(flags));
}

constexpr vk::Format getFormat(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::R8_UNORM:
        return vk::Format::eR8Unorm;
    case TextureFormat::R8_SNORM:
        return vk::Format::eR8Snorm;
    case TextureFormat::R8_USCALED:
        return vk::Format::eR8Uscaled;
    case TextureFormat::R8_SSCALED:
        return vk::Format::eR8Sscaled;
    case TextureFormat::R8_UINT:
        return vk::Format::eR8Uint;
    case TextureFormat::R8_SINT:
        return vk::Format::eR8Sint;
    case TextureFormat::R8_SRGB:
        return vk::Format::eR8Srgb;

    case TextureFormat::R8G8_UNORM:
        return vk::Format::eR8G8Unorm;
    case TextureFormat::R8G8_SNORM:
        return vk::Format::eR8G8Snorm;
    case TextureFormat::R8G8_USCALED:
        return vk::Format::eR8G8Uscaled;
    case TextureFormat::R8G8_SSCALED:
        return vk::Format::eR8G8Sscaled;
    case TextureFormat::R8G8_UINT:
        return vk::Format::eR8G8Uint;
    case TextureFormat::R8G8_SINT:
        return vk::Format::eR8G8Sint;
    case TextureFormat::R8G8_SRGB:
        return vk::Format::eR8G8Srgb;

    case TextureFormat::R8G8B8_UNORM:
        return vk::Format::eR8G8B8Unorm;
    case TextureFormat::R8G8B8_SNORM:
        return vk::Format::eR8G8B8Snorm;
    case TextureFormat::R8G8B8_USCALED:
        return vk::Format::eR8G8B8Uscaled;
    case TextureFormat::R8G8B8_SSCALED:
        return vk::Format::eR8G8B8Sscaled;
    case TextureFormat::R8G8B8_UINT:
        return vk::Format::eR8G8B8Uint;
    case TextureFormat::R8G8B8_SINT:
        return vk::Format::eR8G8B8Sint;
    case TextureFormat::R8G8B8_SRGB:
        return vk::Format::eR8G8B8Srgb;

    case TextureFormat::B8G8R8_UNORM:
        return vk::Format::eB8G8R8Unorm;
    case TextureFormat::B8G8R8_SNORM:
        return vk::Format::eB8G8R8Snorm;
    case TextureFormat::B8G8R8_USCALED:
        return vk::Format::eB8G8R8Uscaled;
    case TextureFormat::B8G8R8_SSCALED:
        return vk::Format::eB8G8R8Sscaled;
    case TextureFormat::B8G8R8_UINT:
        return vk::Format::eB8G8R8Uint;
    case TextureFormat::B8G8R8_SINT:
        return vk::Format::eB8G8R8Sint;
    case TextureFormat::B8G8R8_SRGB:
        return vk::Format::eB8G8R8Srgb;

    case TextureFormat::R8G8B8A8_UNORM:
        return vk::Format::eR8G8B8A8Unorm;
    case TextureFormat::R8G8B8A8_SNORM:
        return vk::Format::eR8G8B8A8Snorm;
    case TextureFormat::R8G8B8A8_USCALED:
        return vk::Format::eR8G8B8A8Uscaled;
    case TextureFormat::R8G8B8A8_SSCALED:
        return vk::Format::eR8G8B8A8Sscaled;
    case TextureFormat::R8G8B8A8_UINT:
        return vk::Format::eR8G8B8A8Uint;
    case TextureFormat::R8G8B8A8_SINT:
        return vk::Format::eR8G8B8A8Sint;
    case TextureFormat::R8G8B8A8_SRGB:
        return vk::Format::eR8G8B8A8Srgb;

    case TextureFormat::B8G8R8A8_UNORM:
        return vk::Format::eB8G8R8A8Unorm;
    case TextureFormat::B8G8R8A8_SNORM:
        return vk::Format::eB8G8R8A8Snorm;
    case TextureFormat::B8G8R8A8_USCALED:
        return vk::Format::eB8G8R8A8Uscaled;
    case TextureFormat::B8G8R8A8_SSCALED:
        return vk::Format::eB8G8R8A8Sscaled;
    case TextureFormat::B8G8R8A8_UINT:
        return vk::Format::eB8G8R8A8Uint;
    case TextureFormat::B8G8R8A8_SINT:
        return vk::Format::eB8G8R8A8Sint;
    case TextureFormat::B8G8R8A8_SRGB:
        return vk::Format::eB8G8R8A8Srgb;

    case TextureFormat::R16_UNORM:
        return vk::Format::eR16Unorm;
    case TextureFormat::R16_SNORM:
        return vk::Format::eR16Snorm;
    case TextureFormat::R16_USCALED:
        return vk::Format::eR16Uscaled;
    case TextureFormat::R16_SSCALED:
        return vk::Format::eR16Sscaled;
    case TextureFormat::R16_UINT:
        return vk::Format::eR16Uint;
    case TextureFormat::R16_SINT:
        return vk::Format::eR16Sint;
    case TextureFormat::R16_SFLOAT:
        return vk::Format::eR16Sfloat;

    case TextureFormat::R16G16_UNORM:
        return vk::Format::eR16G16Unorm;
    case TextureFormat::R16G16_SNORM:
        return vk::Format::eR16G16Snorm;
    case TextureFormat::R16G16_USCALED:
        return vk::Format::eR16G16Uscaled;
    case TextureFormat::R16G16_SSCALED:
        return vk::Format::eR16G16Sscaled;
    case TextureFormat::R16G16_UINT:
        return vk::Format::eR16G16Uint;
    case TextureFormat::R16G16_SINT:
        return vk::Format::eR16G16Sint;
    case TextureFormat::R16G16_SFLOAT:
        return vk::Format::eR16G16Sfloat;

    case TextureFormat::R16G16B16_UNORM:
        return vk::Format::eR16G16B16Unorm;
    case TextureFormat::R16G16B16_SNORM:
        return vk::Format::eR16G16B16Snorm;
    case TextureFormat::R16G16B16_USCALED:
        return vk::Format::eR16G16B16Uscaled;
    case TextureFormat::R16G16B16_SSCALED:
        return vk::Format::eR16G16B16Sscaled;
    case TextureFormat::R16G16B16_UINT:
        return vk::Format::eR16G16B16Uint;
    case TextureFormat::R16G16B16_SINT:
        return vk::Format::eR16G16B16Sint;
    case TextureFormat::R16G16B16_SFLOAT:
        return vk::Format::eR16G16B16Sfloat;

    case TextureFormat::R16G16B16A16_UNORM:
        return vk::Format::eR16G16B16A16Unorm;
    case TextureFormat::R16G16B16A16_SNORM:
        return vk::Format::eR16G16B16A16Snorm;
    case TextureFormat::R16G16B16A16_USCALED:
        return vk::Format::eR16G16B16A16Uscaled;
    case TextureFormat::R16G16B16A16_SSCALED:
        return vk::Format::eR16G16B16A16Sscaled;
    case TextureFormat::R16G16B16A16_UINT:
        return vk::Format::eR16G16B16A16Uint;
    case TextureFormat::R16G16B16A16_SINT:
        return vk::Format::eR16G16B16A16Sint;
    case TextureFormat::R16G16B16A16_SFLOAT:
        return vk::Format::eR16G16B16A16Sfloat;

    case TextureFormat::R32_UINT:
        return vk::Format::eR32Uint;
    case TextureFormat::R32_SINT:
        return vk::Format::eR32Sint;
    case TextureFormat::R32_SFLOAT:
        return vk::Format::eR32Sfloat;

    case TextureFormat::R32G32_UINT:
        return vk::Format::eR32G32Uint;
    case TextureFormat::R32G32_SINT:
        return vk::Format::eR32G32Sint;
    case TextureFormat::R32G32_SFLOAT:
        return vk::Format::eR32G32Sfloat;

    case TextureFormat::R32G32B32_UINT:
        return vk::Format::eR32G32B32Uint;
    case TextureFormat::R32G32B32_SINT:
        return vk::Format::eR32G32B32Sint;
    case TextureFormat::R32G32B32_SFLOAT:
        return vk::Format::eR32G32B32Sfloat;

    case TextureFormat::R32G32B32A32_UINT:
        return vk::Format::eR32G32B32A32Uint;
    case TextureFormat::R32G32B32A32_SINT:
        return vk::Format::eR32G32B32A32Sint;
    case TextureFormat::R32G32B32A32_SFLOAT:
        return vk::Format::eR32G32B32A32Sfloat;

    case TextureFormat::R64_UINT:
        return vk::Format::eR64Uint;
    case TextureFormat::R64_SINT:
        return vk::Format::eR64Sint;
    case TextureFormat::R64_SFLOAT:
        return vk::Format::eR64Sfloat;

    case TextureFormat::R64G64_UINT:
        return vk::Format::eR64G64Uint;
    case TextureFormat::R64G64_SINT:
        return vk::Format::eR64G64Sint;
    case TextureFormat::R64G64_SFLOAT:
        return vk::Format::eR64G64Sfloat;

    case TextureFormat::R64G64B64_UINT:
        return vk::Format::eR64G64B64Uint;
    case TextureFormat::R64G64B64_SINT:
        return vk::Format::eR64G64B64Sint;
    case TextureFormat::R64G64B64_SFLOAT:
        return vk::Format::eR64G64B64Sfloat;

    case TextureFormat::R64G64B64A64_UINT:
        return vk::Format::eR64G64B64A64Uint;
    case TextureFormat::R64G64B64A64_SINT:
        return vk::Format::eR64G64B64A64Sint;
    case TextureFormat::R64G64B64A64_SFLOAT:
        return vk::Format::eR64G64B64A64Sfloat;

    case TextureFormat::D16_UNORM:
        return vk::Format::eD16Unorm;
    case TextureFormat::D32_SFLOAT:
        return vk::Format::eD32Sfloat;
    case TextureFormat::S8_UINT:
        return vk::Format::eS8Uint;
    case TextureFormat::D16_UNORM_S8_UINT:
        return vk::Format::eD16UnormS8Uint;
    case TextureFormat::D24_UNORM_S8_UINT:
        return vk::Format::eD24UnormS8Uint;
    case TextureFormat::D32_SFLOAT_S8_UINT:
        return vk::Format::eD32SfloatS8Uint;
    case TextureFormat::UNKNOWN:
    default:
        return vk::Format::eUndefined;
    }
}

constexpr bool isDepthTextureFormat(const vk::Format format)
{
    if (format == vk::Format::eD16Unorm
        || format == vk::Format::eD32Sfloat
        || format == vk::Format::eD16UnormS8Uint
        || format == vk::Format::eD24UnormS8Uint
        || format == vk::Format::eD32SfloatS8Uint)
    {
        return true;
    }
    return false;
}

constexpr uint8_t getPixelSize(const vk::Format format)
{
    switch (format)
    {
    case vk::Format::eR8Unorm:
    case vk::Format::eR8Snorm:
    case vk::Format::eR8Uscaled:
    case vk::Format::eR8Sscaled:
    case vk::Format::eR8Uint:
    case vk::Format::eR8Sint:
    case vk::Format::eR8Srgb:
        return 1;
    case vk::Format::eR8G8Unorm:
    case vk::Format::eR8G8Snorm:
    case vk::Format::eR8G8Uscaled:
    case vk::Format::eR8G8Sscaled:
    case vk::Format::eR8G8Uint:
    case vk::Format::eR8G8Sint:
    case vk::Format::eR8G8Srgb:
        return 2;
    case vk::Format::eR8G8B8Unorm:
    case vk::Format::eR8G8B8Snorm:
    case vk::Format::eR8G8B8Uscaled:
    case vk::Format::eR8G8B8Sscaled:
    case vk::Format::eR8G8B8Uint:
    case vk::Format::eR8G8B8Sint:
    case vk::Format::eR8G8B8Srgb:
    case vk::Format::eB8G8R8Unorm:
    case vk::Format::eB8G8R8Snorm:
    case vk::Format::eB8G8R8Uscaled:
    case vk::Format::eB8G8R8Sscaled:
    case vk::Format::eB8G8R8Uint:
    case vk::Format::eB8G8R8Sint:
    case vk::Format::eB8G8R8Srgb:
        return 3;
    case vk::Format::eR8G8B8A8Unorm:
    case vk::Format::eR8G8B8A8Snorm:
    case vk::Format::eR8G8B8A8Uscaled:
    case vk::Format::eR8G8B8A8Sscaled:
    case vk::Format::eR8G8B8A8Uint:
    case vk::Format::eR8G8B8A8Sint:
    case vk::Format::eR8G8B8A8Srgb:
    case vk::Format::eB8G8R8A8Unorm:
    case vk::Format::eB8G8R8A8Snorm:
    case vk::Format::eB8G8R8A8Uscaled:
    case vk::Format::eB8G8R8A8Sscaled:
    case vk::Format::eB8G8R8A8Uint:
    case vk::Format::eB8G8R8A8Sint:
    case vk::Format::eB8G8R8A8Srgb:
        return 4;
    case vk::Format::eR16Unorm:
    case vk::Format::eR16Snorm:
    case vk::Format::eR16Uscaled:
    case vk::Format::eR16Sscaled:
    case vk::Format::eR16Uint:
    case vk::Format::eR16Sint:
    case vk::Format::eR16Sfloat:
        return 2;
    case vk::Format::eR16G16Unorm:
    case vk::Format::eR16G16Snorm:
    case vk::Format::eR16G16Uscaled:
    case vk::Format::eR16G16Sscaled:
    case vk::Format::eR16G16Uint:
    case vk::Format::eR16G16Sint:
    case vk::Format::eR16G16Sfloat:
        return 4;
    case vk::Format::eR16G16B16Unorm:
    case vk::Format::eR16G16B16Snorm:
    case vk::Format::eR16G16B16Uscaled:
    case vk::Format::eR16G16B16Sscaled:
    case vk::Format::eR16G16B16Uint:
    case vk::Format::eR16G16B16Sint:
    case vk::Format::eR16G16B16Sfloat:
        return 6;
    case vk::Format::eR16G16B16A16Unorm:
    case vk::Format::eR16G16B16A16Snorm:
    case vk::Format::eR16G16B16A16Uscaled:
    case vk::Format::eR16G16B16A16Sscaled:
    case vk::Format::eR16G16B16A16Uint:
    case vk::Format::eR16G16B16A16Sint:
    case vk::Format::eR16G16B16A16Sfloat:
        return 8;
    case vk::Format::eR32Uint:
    case vk::Format::eR32Sint:
    case vk::Format::eR32Sfloat:
        return 4;
    case vk::Format::eR32G32Uint:
    case vk::Format::eR32G32Sint:
    case vk::Format::eR32G32Sfloat:
        return 8;
    case vk::Format::eR32G32B32Uint:
    case vk::Format::eR32G32B32Sint:
    case vk::Format::eR32G32B32Sfloat:
        return 12;
    case vk::Format::eR32G32B32A32Uint:
    case vk::Format::eR32G32B32A32Sint:
    case vk::Format::eR32G32B32A32Sfloat:
        return 16;
    case vk::Format::eR64Uint:
    case vk::Format::eR64Sint:
    case vk::Format::eR64Sfloat:
        return 8;
    case vk::Format::eR64G64Uint:
    case vk::Format::eR64G64Sint:
    case vk::Format::eR64G64Sfloat:
        return 16;
    case vk::Format::eR64G64B64Uint:
    case vk::Format::eR64G64B64Sint:
    case vk::Format::eR64G64B64Sfloat:
        return 24;
    case vk::Format::eR64G64B64A64Uint:
    case vk::Format::eR64G64B64A64Sint:
    case vk::Format::eR64G64B64A64Sfloat:
        return 32;
    case vk::Format::eD16Unorm:
        return 2;
    case vk::Format::eD32Sfloat:
        return 4;
    case vk::Format::eS8Uint:
        return 1;
    case vk::Format::eD16UnormS8Uint:
        return 3;
    case vk::Format::eD24UnormS8Uint:
        return 4;
    case vk::Format::eD32SfloatS8Uint:
        return 5;
    case vk::Format::eUndefined:
    default:
        return 0;
    }
}

constexpr vk::ImageLayout getImageLayout(const TextureLayout layout)
{
    switch (layout)
    {
    case TextureLayout::GENERAL:
        return vk::ImageLayout::eGeneral;
    case TextureLayout::COLOR_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eColorAttachmentOptimal;
    case TextureLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eDepthStencilAttachmentOptimal;
    case TextureLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
    case TextureLayout::SHADER_READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eShaderReadOnlyOptimal;
    case TextureLayout::TRANSFER_SRC_OPTIMAL:
        return vk::ImageLayout::eTransferSrcOptimal;
    case TextureLayout::TRANSFER_DST_OPTIMAL:
        return vk::ImageLayout::eTransferDstOptimal;
    case TextureLayout::PREINITIALIZED:
        return vk::ImageLayout::ePreinitialized;
    case TextureLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;
    case TextureLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal;
    case TextureLayout::DEPTH_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eDepthAttachmentOptimal;
    case TextureLayout::DEPTH_READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eDepthReadOnlyOptimal;
    case TextureLayout::STENCIL_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eStencilAttachmentOptimal;
    case TextureLayout::STENCIL_READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eStencilReadOnlyOptimal;
    case TextureLayout::READ_ONLY_OPTIMAL:
        return vk::ImageLayout::eReadOnlyOptimal;
    case TextureLayout::ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eAttachmentOptimal;
    case TextureLayout::RENDERING_LOCAL_READ:
        return vk::ImageLayout::eRenderingLocalRead;
    case TextureLayout::PRESENT_SRC:
        return vk::ImageLayout::ePresentSrcKHR;
    case TextureLayout::VIDEO_DECODE_DST:
        return vk::ImageLayout::eVideoDecodeDstKHR;
    case TextureLayout::VIDEO_DECODE_SRC:
        return vk::ImageLayout::eVideoDecodeSrcKHR;
    case TextureLayout::VIDEO_DECODE_DPB:
        return vk::ImageLayout::eVideoDecodeDpbKHR;
    case TextureLayout::SHARED_PRESENT:
        return vk::ImageLayout::eSharedPresentKHR;
    case TextureLayout::FRAGMENT_DENSITY_MAP_OPTIMAL:
        return vk::ImageLayout::eFragmentDensityMapOptimalEXT;
    case TextureLayout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL:
        return vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
    case TextureLayout::SHADING_RATE_OPTIMAL:
        return vk::ImageLayout::eShadingRateOptimalNV;
    case TextureLayout::VIDEO_ENCODE_DST:
        return vk::ImageLayout::eVideoEncodeDstKHR;
    case TextureLayout::VIDEO_ENCODE_SRC:
        return vk::ImageLayout::eVideoEncodeSrcKHR;
    case TextureLayout::VIDEO_ENCODE_DPB:
        return vk::ImageLayout::eVideoEncodeDpbKHR;
    case TextureLayout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL:
        return vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT;
    case TextureLayout::VIDEO_ENCODE_QUANTIZATION_MAP:
        return vk::ImageLayout::eVideoEncodeQuantizationMapKHR;
    case TextureLayout::UNDEFINED:
    default:
        return vk::ImageLayout::eUndefined;
    }
}

constexpr TextureLayout getNativeTextureLayout(const vk::ImageLayout layout)
{
    switch (layout)
    {
    case vk::ImageLayout::eGeneral:
        return TextureLayout::GENERAL;
    case vk::ImageLayout::eColorAttachmentOptimal:
        return TextureLayout::COLOR_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eDepthStencilAttachmentOptimal:
        return TextureLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eDepthStencilReadOnlyOptimal:
        return TextureLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eShaderReadOnlyOptimal:
        return TextureLayout::SHADER_READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eTransferSrcOptimal:
        return TextureLayout::TRANSFER_SRC_OPTIMAL;
    case vk::ImageLayout::eTransferDstOptimal:
        return TextureLayout::TRANSFER_DST_OPTIMAL;
    case vk::ImageLayout::ePreinitialized:
        return TextureLayout::PREINITIALIZED;
    case vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal:
        return TextureLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal:
        return TextureLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eDepthAttachmentOptimal:
        return TextureLayout::DEPTH_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eDepthReadOnlyOptimal:
        return TextureLayout::DEPTH_READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eStencilAttachmentOptimal:
        return TextureLayout::STENCIL_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eStencilReadOnlyOptimal:
        return TextureLayout::STENCIL_READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eReadOnlyOptimal:
        return TextureLayout::READ_ONLY_OPTIMAL;
    case vk::ImageLayout::eAttachmentOptimal:
        return TextureLayout::ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eRenderingLocalRead:
        return TextureLayout::RENDERING_LOCAL_READ;
    case vk::ImageLayout::ePresentSrcKHR:
        return TextureLayout::PRESENT_SRC;
    case vk::ImageLayout::eVideoDecodeDstKHR:
        return TextureLayout::VIDEO_DECODE_DST;
    case vk::ImageLayout::eVideoDecodeSrcKHR:
        return TextureLayout::VIDEO_DECODE_SRC;
    case vk::ImageLayout::eVideoDecodeDpbKHR:
        return TextureLayout::VIDEO_DECODE_DPB;
    case vk::ImageLayout::eSharedPresentKHR:
        return TextureLayout::SHARED_PRESENT;
    case vk::ImageLayout::eFragmentDensityMapOptimalEXT:
        return TextureLayout::FRAGMENT_DENSITY_MAP_OPTIMAL;
    case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR:
        return TextureLayout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL;
    case vk::ImageLayout::eVideoEncodeDstKHR:
        return TextureLayout::VIDEO_ENCODE_DST;
    case vk::ImageLayout::eVideoEncodeSrcKHR:
        return TextureLayout::VIDEO_ENCODE_SRC;
    case vk::ImageLayout::eVideoEncodeDpbKHR:
        return TextureLayout::VIDEO_ENCODE_DPB;
    case vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT:
        return TextureLayout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL;
    case vk::ImageLayout::eUndefined:
    default:
        return TextureLayout::UNDEFINED;
    }
}

constexpr vk::ImageUsageFlags getImageUsageFlags(const TextureUsageFlags flags)
{
    return static_cast<vk::ImageUsageFlags>(static_cast<uint32_t>(flags));
}

constexpr SampleCountFlags getSampleCountFlags(const vk::SampleCountFlags flags)
{
    return static_cast<SampleCountFlags>(static_cast<uint32_t>(flags));
}

constexpr vk::Filter getFilter(const SamplerFilterMode filter_mode)
{
    return static_cast<vk::Filter>(static_cast<uint8_t>(filter_mode));
}

constexpr vk::SamplerMipmapMode getSamplerMipmapMode(const SamplerFilterMode filter_mode)
{
    return static_cast<vk::SamplerMipmapMode>(static_cast<uint8_t>(filter_mode));
}

constexpr vk::SamplerAddressMode getSamplerAddressMode(const SamplerWrapMode wrap_mode)
{
    return static_cast<vk::SamplerAddressMode>(static_cast<uint8_t>(wrap_mode));
}

constexpr GPUType getGPUType(const vk::PhysicalDeviceType type)
{
    switch (type)
    {
    case vk::PhysicalDeviceType::eIntegratedGpu:
        return GPUType::INTEGRATED;
    case vk::PhysicalDeviceType::eDiscreteGpu:
        return GPUType::DISCRETE;
    default:
        return GPUType::UNKNOWN;
    }
}

constexpr vk::ShaderStageFlags getShaderStageFlags(const ShaderStageFlags flags)
{
    return static_cast<vk::ShaderStageFlags>(static_cast<uint32_t>(flags));
}

constexpr vk::DescriptorType getDescriptorType(const ShaderBindingType type)
{
    switch (type)
    {
    case ShaderBindingType::SAMPLER:
        return vk::DescriptorType::eSampler;
    case ShaderBindingType::COMBINED_IMAGE_SAMPLER:
        return vk::DescriptorType::eCombinedImageSampler;
    case ShaderBindingType::SAMPLED_IMAGE:
        return vk::DescriptorType::eSampledImage;
    case ShaderBindingType::STORAGE_IMAGE:
        return vk::DescriptorType::eStorageImage;
    case ShaderBindingType::UNIFORM_TEXEL_BUFFER:
        return vk::DescriptorType::eUniformTexelBuffer;
    case ShaderBindingType::STORAGE_TEXEL_BUFFER:
        return vk::DescriptorType::eStorageTexelBuffer;
    case ShaderBindingType::UNIFORM_BUFFER:
        return vk::DescriptorType::eUniformBuffer;
    case ShaderBindingType::STORAGE_BUFFER:
        return vk::DescriptorType::eStorageBuffer;
    case ShaderBindingType::UNIFORM_BUFFER_DYNAMIC:
        return vk::DescriptorType::eUniformBufferDynamic;
    case ShaderBindingType::STORAGE_BUFFER_DYNAMIC:
        return vk::DescriptorType::eStorageBufferDynamic;
    case ShaderBindingType::INPUT_ATTACHMENT:
        return vk::DescriptorType::eInputAttachment;
    case ShaderBindingType::INLINE_UNIFORM_BLOCK:
        return vk::DescriptorType::eInlineUniformBlock;
    case ShaderBindingType::ACCELERATION_STRUCTURE:
        return vk::DescriptorType::eAccelerationStructureKHR;
    case ShaderBindingType::MUTABLE:
        return vk::DescriptorType::eMutableEXT;
    case ShaderBindingType::UNKNOWN:
    default:
        return vk::DescriptorType::eUniformBuffer;
    }
}

constexpr vk::AccessFlags2 getAccessFlags(const MemoryAccessFlags flags)
{
    return static_cast<vk::AccessFlags2>(static_cast<uint64_t>(flags));
}

constexpr vk::PipelineStageFlags2 getPipelineStageFlags(const PipelineStageFlags flags)
{
    return static_cast<vk::PipelineStageFlags2>(static_cast<uint64_t>(flags));
}

constexpr vk::PipelineBindPoint getPipelineBindPoint(const PipelineType type)
{
    switch (type)
    {
    case PipelineType::COMPUTE:
        return vk::PipelineBindPoint::eCompute;
    case PipelineType::RAY_TRACING:
        return vk::PipelineBindPoint::eRayTracingKHR;
    case PipelineType::GRAPHICS:
    default:
        return vk::PipelineBindPoint::eGraphics;
    }
}

constexpr QueueFamilyFlags getQueueFamilyFlags(const vk::QueueFlags flags,
                                               const bool presentation_support)
{
    auto res = QueueFamilyFlags::NONE;
    if ((flags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
    {
        res = res | QueueFamilyFlags::GRAPHICS;
    }
    if ((flags & vk::QueueFlagBits::eCompute) == vk::QueueFlagBits::eCompute)
    {
        res = res | QueueFamilyFlags::COMPUTE;
    }
    if ((flags & vk::QueueFlagBits::eTransfer) == vk::QueueFlagBits::eTransfer)
    {
        res = res | QueueFamilyFlags::TRANSFER;
    }
    if (presentation_support)
    {
        res = res | QueueFamilyFlags::PRESENTATION;
    }
    return res;
}

inline std::vector<QueueFamily> getQueueFamilies(const vk::PhysicalDevice device, const ::vk::SurfaceKHR surface)
{
    const std::vector<vk::QueueFamilyProperties> props =
        device.getQueueFamilyProperties();
    std::vector<QueueFamily> queue_families;
    for (uint32_t i = 0; i < props.size(); i++)
    {
        const bool presentation_supported =
            surface != nullptr &&
            device.getSurfaceSupportKHR(i, surface);
        queue_families.emplace_back(QueueFamily{
            i, props[i].queueCount,
            getQueueFamilyFlags(props[i].queueFlags, presentation_supported)
        });
    }
    return queue_families;
}

constexpr vk::PresentModeKHR getPresentMode(const PresentMode mode)
{
    switch (mode)
    {
    case PresentMode::MAILBOX:
        return vk::PresentModeKHR::eMailbox;
    case PresentMode::FIFO:
        return vk::PresentModeKHR::eFifo;
    case PresentMode::FIFO_RELAXED:
        return vk::PresentModeKHR::eFifoRelaxed;
    case PresentMode::IMMEDIATE:
    default:
        return vk::PresentModeKHR::eImmediate;
    }
}

inline GPUFeatures getGPUFeatures(const vk::PhysicalDevice device)
{
    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_features = {};
    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accel_struct_features = {};
    accel_struct_features.pNext = &raytracing_features;
    vk::PhysicalDeviceVulkan14Features features_1_4 = {};
    features_1_4.pNext = &accel_struct_features;
    vk::PhysicalDeviceVulkan13Features features_1_3 = {};
    features_1_3.pNext = &features_1_4;
    vk::PhysicalDeviceVulkan12Features features_1_2 = {};
    features_1_2.pNext = &features_1_3;
    vk::PhysicalDeviceVulkan11Features features_1_1 = {};
    features_1_1.pNext = &features_1_2;
    vk::PhysicalDeviceFeatures2 features = {};
    features.pNext = &features_1_1;

    device.getFeatures2(&features);

    GPUFeatures gpu_features = {};
    gpu_features.robust_buffer_access = features.features.robustBufferAccess;
    gpu_features.full_draw_index_uint_32 = features.features.fullDrawIndexUint32;
    gpu_features.image_cube_array = features.features.imageCubeArray;
    gpu_features.independent_blend = features.features.independentBlend;
    gpu_features.geometry_shader = features.features.geometryShader;
    gpu_features.tessellation_shader = features.features.tessellationShader;
    gpu_features.sample_rate_shading = features.features.sampleRateShading;
    gpu_features.dual_src_blend = features.features.dualSrcBlend;
    gpu_features.logic_op = features.features.logicOp;
    gpu_features.multi_draw_indirect = features.features.multiDrawIndirect;
    gpu_features.draw_indirect_first_instance = features.features.drawIndirectFirstInstance;
    gpu_features.depth_clamp = features.features.depthClamp;
    gpu_features.depth_bias_clamp = features.features.depthBiasClamp;
    gpu_features.fill_mode_non_solid = features.features.fillModeNonSolid;
    gpu_features.depth_bounds = features.features.depthBounds;
    gpu_features.wide_lines = features.features.wideLines;
    gpu_features.large_points = features.features.largePoints;
    gpu_features.alpha_to_one = features.features.alphaToOne;
    gpu_features.multi_viewport = features.features.multiViewport;
    gpu_features.sampler_anisotropy = features.features.samplerAnisotropy;
    gpu_features.texture_compression_ETC2 = features.features.textureCompressionETC2;
    gpu_features.texture_compression_ASTCLDR = features.features.textureCompressionASTC_LDR;
    gpu_features.texture_compression_BC = features.features.textureCompressionBC;
    gpu_features.occlusion_query_precise = features.features.occlusionQueryPrecise;
    gpu_features.pipeline_statistics_query = features.features.pipelineStatisticsQuery;
    gpu_features.vertex_pipeline_stores_and_atomics = features.features.vertexPipelineStoresAndAtomics;
    gpu_features.fragment_stores_and_atomics = features.features.fragmentStoresAndAtomics;
    gpu_features.shader_tessellation_and_geometry_point_size = features.features.shaderTessellationAndGeometryPointSize;
    gpu_features.shader_image_gather_extended = features.features.shaderImageGatherExtended;
    gpu_features.shader_storage_image_extended_formats = features.features.shaderStorageImageExtendedFormats;
    gpu_features.shader_storage_image_multisample = features.features.shaderStorageImageMultisample;
    gpu_features.shader_storage_image_read_without_format = features.features.shaderStorageImageReadWithoutFormat;
    gpu_features.shader_storage_image_write_without_format = features.features.shaderStorageImageWriteWithoutFormat;
    gpu_features.shader_uniform_buffer_array_dynamic_indexing = features.features.
                                                                         shaderUniformBufferArrayDynamicIndexing;
    gpu_features.shader_sampled_image_array_dynamic_indexing = features.features.shaderSampledImageArrayDynamicIndexing;
    gpu_features.shader_storage_buffer_array_dynamic_indexing = features.features.
                                                                         shaderStorageBufferArrayDynamicIndexing;
    gpu_features.shader_storage_image_array_dynamic_indexing = features.features.shaderStorageImageArrayDynamicIndexing;
    gpu_features.shader_clip_distance = features.features.shaderClipDistance;
    gpu_features.shader_cull_distance = features.features.shaderCullDistance;
    gpu_features.shader_float_64 = features.features.shaderFloat64;
    gpu_features.shader_int_64 = features.features.shaderInt64;
    gpu_features.shader_int_16 = features.features.shaderInt16;
    gpu_features.shader_resource_residency = features.features.shaderResourceResidency;
    gpu_features.shader_resource_min_lod = features.features.shaderResourceMinLod;
    gpu_features.sparse_binding = features.features.sparseBinding;
    gpu_features.sparse_residency_buffer = features.features.sparseResidencyBuffer;
    gpu_features.sparse_residency_image_2D = features.features.sparseResidencyImage2D;
    gpu_features.sparse_residency_image_3D = features.features.sparseResidencyImage3D;
    gpu_features.sparse_residency_2_samples = features.features.sparseResidency2Samples;
    gpu_features.sparse_residency_4_samples = features.features.sparseResidency4Samples;
    gpu_features.sparse_residency_8_samples = features.features.sparseResidency8Samples;
    gpu_features.sparse_residency_16_samples = features.features.sparseResidency16Samples;
    gpu_features.sparse_residency_aliased = features.features.sparseResidencyAliased;
    gpu_features.variable_multisample_rate = features.features.variableMultisampleRate;
    gpu_features.inherited_queries = features.features.inheritedQueries;
    // Vulkan 1.1 features
    gpu_features.storage_buffer_16_bit_access = features_1_1.storageBuffer16BitAccess;
    gpu_features.uniform_and_storage_buffer_16_bit_access = features_1_1.uniformAndStorageBuffer16BitAccess;
    gpu_features.storage_push_constant_16 = features_1_1.storagePushConstant16;
    gpu_features.storage_input_output_16 = features_1_1.storageInputOutput16;
    gpu_features.multiview = features_1_1.multiview;
    gpu_features.multiview_geometry_shader = features_1_1.multiviewGeometryShader;
    gpu_features.multiview_tessellation_shader = features_1_1.multiviewTessellationShader;
    gpu_features.variable_pointers_storage_buffer = features_1_1.variablePointersStorageBuffer;
    gpu_features.variable_pointers = features_1_1.variablePointers;
    gpu_features.protected_memory = features_1_1.protectedMemory;
    gpu_features.sampler_YCBCR_conversion = features_1_1.samplerYcbcrConversion;
    gpu_features.shader_draw_parameters = features_1_1.shaderDrawParameters;
    // Vulkan 1.2 features
    gpu_features.sampler_mirror_clamp_to_edge = features_1_2.samplerMirrorClampToEdge;
    gpu_features.draw_indirect_count = features_1_2.drawIndirectCount;
    gpu_features.storage_buffer_8_bit_access = features_1_2.storageBuffer8BitAccess;
    gpu_features.uniform_and_storage_buffer_8_bit_access = features_1_2.uniformAndStorageBuffer8BitAccess;
    gpu_features.storage_push_constant_8 = features_1_2.storagePushConstant8;
    gpu_features.shader_buffer_int_64_atomics = features_1_2.shaderBufferInt64Atomics;
    gpu_features.shader_shared_int_64_atomics = features_1_2.shaderSharedInt64Atomics;
    gpu_features.shader_float_16 = features_1_2.shaderFloat16;
    gpu_features.shader_int_8 = features_1_2.shaderInt8;
    gpu_features.descriptor_indexing = features_1_2.descriptorIndexing;
    gpu_features.shader_input_attachment_array_dynamic_indexing = features_1_2.
        shaderInputAttachmentArrayDynamicIndexing;
    gpu_features.shader_uniform_texel_buffer_array_dynamic_indexing = features_1_2.
        shaderUniformTexelBufferArrayDynamicIndexing;
    gpu_features.shader_storage_texel_buffer_array_dynamic_indexing = features_1_2.
        shaderStorageTexelBufferArrayDynamicIndexing;
    gpu_features.shader_uniform_buffer_array_non_uniform_indexing = features_1_2.
        shaderUniformBufferArrayNonUniformIndexing;
    gpu_features.shader_sampled_image_array_non_uniform_indexing = features_1_2.
        shaderSampledImageArrayNonUniformIndexing;
    gpu_features.shader_storage_buffer_array_non_uniform_indexing = features_1_2.
        shaderStorageBufferArrayNonUniformIndexing;
    gpu_features.shader_storage_image_array_non_uniform_indexing = features_1_2.
        shaderStorageImageArrayNonUniformIndexing;
    gpu_features.shader_input_attachment_array_non_uniform_indexing = features_1_2.
        shaderInputAttachmentArrayNonUniformIndexing;
    gpu_features.shader_uniform_texel_buffer_array_non_uniform_indexing = features_1_2.
        shaderUniformTexelBufferArrayNonUniformIndexing;
    gpu_features.shader_storage_texel_buffer_array_non_uniform_indexing = features_1_2.
        shaderStorageTexelBufferArrayNonUniformIndexing;
    gpu_features.descriptor_binding_uniform_buffer_update_after_bind = features_1_2.
        descriptorBindingUniformBufferUpdateAfterBind;
    gpu_features.descriptor_binding_sampled_image_update_after_bind = features_1_2.
        descriptorBindingSampledImageUpdateAfterBind;
    gpu_features.descriptor_binding_storage_image_update_after_bind = features_1_2.
        descriptorBindingStorageImageUpdateAfterBind;
    gpu_features.descriptor_binding_storage_buffer_update_after_bind = features_1_2.
        descriptorBindingStorageBufferUpdateAfterBind;
    gpu_features.descriptor_binding_uniform_texel_buffer_update_after_bind = features_1_2.
        descriptorBindingUniformTexelBufferUpdateAfterBind;
    gpu_features.descriptor_binding_storage_texel_buffer_update_after_bind = features_1_2.
        descriptorBindingStorageTexelBufferUpdateAfterBind;
    gpu_features.descriptor_binding_update_unused_while_pending = features_1_2.
        descriptorBindingUpdateUnusedWhilePending;
    gpu_features.descriptor_binding_partially_bound = features_1_2.descriptorBindingPartiallyBound;
    gpu_features.descriptor_binding_variable_descriptor_count = features_1_2.descriptorBindingVariableDescriptorCount;
    gpu_features.runtime_descriptor_array = features_1_2.runtimeDescriptorArray;
    gpu_features.sampler_filter_minmax = features_1_2.samplerFilterMinmax;
    gpu_features.scalar_block_layout = features_1_2.scalarBlockLayout;
    gpu_features.imageless_framebuffer = features_1_2.imagelessFramebuffer;
    gpu_features.uniform_buffer_standard_layout = features_1_2.uniformBufferStandardLayout;
    gpu_features.shader_subgroup_extended_types = features_1_2.shaderSubgroupExtendedTypes;
    gpu_features.separate_depth_stencil_layouts = features_1_2.separateDepthStencilLayouts;
    gpu_features.host_query_reset = features_1_2.hostQueryReset;
    gpu_features.timeline_semaphore = features_1_2.timelineSemaphore;
    gpu_features.buffer_device_address = features_1_2.bufferDeviceAddress;
    gpu_features.buffer_device_address_capture_replay = features_1_2.bufferDeviceAddressCaptureReplay;
    gpu_features.buffer_device_address_multi_device = features_1_2.bufferDeviceAddressMultiDevice;
    gpu_features.vulkan_memory_model = features_1_2.vulkanMemoryModel;
    gpu_features.vulkan_memory_model_device_scope = features_1_2.vulkanMemoryModelDeviceScope;
    gpu_features.vulkan_memory_model_availability_visibility_chains = features_1_2.
        vulkanMemoryModelAvailabilityVisibilityChains;
    gpu_features.shader_output_viewport_index = features_1_2.shaderOutputViewportIndex;
    gpu_features.shader_output_layer = features_1_2.shaderOutputLayer;
    gpu_features.subgroup_broadcast_dynamic_id = features_1_2.subgroupBroadcastDynamicId;
    // Vulkan 1.3 features
    gpu_features.robust_image_access = features_1_3.robustImageAccess;
    gpu_features.inline_uniform_block = features_1_3.inlineUniformBlock;
    gpu_features.descriptor_binding_inline_uniform_block_update_after_bind = features_1_3.
        descriptorBindingInlineUniformBlockUpdateAfterBind;
    gpu_features.pipeline_creation_cache_control = features_1_3.pipelineCreationCacheControl;
    gpu_features.private_data = features_1_3.privateData;
    gpu_features.shader_demote_to_helper_invocation = features_1_3.shaderDemoteToHelperInvocation;
    gpu_features.shader_terminate_invocation = features_1_3.shaderTerminateInvocation;
    gpu_features.subgroup_size_control = features_1_3.subgroupSizeControl;
    gpu_features.compute_full_subgroups = features_1_3.computeFullSubgroups;
    gpu_features.synchronization_2 = features_1_3.synchronization2;
    gpu_features.texture_compression_ASTC_HDR = features_1_3.textureCompressionASTC_HDR;
    gpu_features.shader_zero_initialize_workgroup_memory = features_1_3.shaderZeroInitializeWorkgroupMemory;
    gpu_features.dynamic_rendering = features_1_3.dynamicRendering;
    gpu_features.shader_integer_dot_product = features_1_3.shaderIntegerDotProduct;
    gpu_features.maintenance_4 = features_1_3.maintenance4;
    // Vulkan 1.4 features
    gpu_features.global_priority_query = features_1_4.globalPriorityQuery;
    gpu_features.shader_subgroup_rotate = features_1_4.shaderSubgroupRotate;
    gpu_features.shader_subgroup_rotate_clustered = features_1_4.shaderSubgroupRotateClustered;
    gpu_features.shader_float_controls_2 = features_1_4.shaderFloatControls2;
    gpu_features.shader_expect_assume = features_1_4.shaderExpectAssume;
    gpu_features.rectangular_lines = features_1_4.rectangularLines;
    gpu_features.bresenham_lines = features_1_4.bresenhamLines;
    gpu_features.smooth_lines = features_1_4.smoothLines;
    gpu_features.stippled_rectangular_lines = features_1_4.stippledRectangularLines;
    gpu_features.stippled_bresenham_lines = features_1_4.stippledBresenhamLines;
    gpu_features.stippled_smooth_lines = features_1_4.stippledSmoothLines;
    gpu_features.vertex_attribute_instance_rate_divisor = features_1_4.vertexAttributeInstanceRateDivisor;
    gpu_features.vertex_attribute_instance_rate_zero_divisor = features_1_4.vertexAttributeInstanceRateZeroDivisor;
    gpu_features.index_type_uint_8 = features_1_4.indexTypeUint8;
    gpu_features.dynamic_rendering_local_read = features_1_4.dynamicRenderingLocalRead;
    gpu_features.maintenance_5 = features_1_4.maintenance5;
    gpu_features.maintenance_6 = features_1_4.maintenance6;
    gpu_features.pipeline_protected_access = features_1_4.pipelineProtectedAccess;
    gpu_features.pipeline_robustness = features_1_4.pipelineRobustness;
    gpu_features.host_image_copy = features_1_4.hostImageCopy;
    gpu_features.push_descriptor = features_1_4.pushDescriptor;
    // Acceleration Structure features
    gpu_features.acceleration_structure = accel_struct_features.accelerationStructure;
    gpu_features.acceleration_structure_capture_replay = accel_struct_features.accelerationStructureCaptureReplay;
    gpu_features.acceleration_structure_indirect_build = accel_struct_features.accelerationStructureIndirectBuild;
    gpu_features.acceleration_structure_host_commands = accel_struct_features.accelerationStructureHostCommands;
    gpu_features.descriptor_binding_acceleration_structure_update_after_bind = accel_struct_features.
        descriptorBindingAccelerationStructureUpdateAfterBind;
    // Raytracing features
    gpu_features.ray_tracing_pipeline = raytracing_features.rayTracingPipeline;
    gpu_features.ray_tracing_pipeline_shader_group_handle_capture_replay = raytracing_features.
        rayTracingPipelineShaderGroupHandleCaptureReplay;
    gpu_features.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed = raytracing_features.
        rayTracingPipelineShaderGroupHandleCaptureReplayMixed;
    gpu_features.ray_tracing_pipeline_trace_rays_indirect = raytracing_features.rayTracingPipelineTraceRaysIndirect;
    gpu_features.ray_traversal_primitive_culling = raytracing_features.rayTraversalPrimitiveCulling;
    return gpu_features;
}

inline GPULimits getGPULimits(const vk::PhysicalDevice device)
{
    vk::PhysicalDeviceRayTracingPipelinePropertiesKHR raytracing_props = {};
    vk::PhysicalDeviceAccelerationStructurePropertiesKHR accel_struct_props = {};
    accel_struct_props.pNext = &raytracing_props;
    vk::PhysicalDeviceVulkan14Properties props_1_4 = {};
    props_1_4.pNext = &accel_struct_props;
    vk::PhysicalDeviceVulkan13Properties props_1_3 = {};
    props_1_3.pNext = &props_1_4;
    vk::PhysicalDeviceVulkan12Properties props_1_2 = {};
    props_1_2.pNext = &props_1_3;
    vk::PhysicalDeviceVulkan11Properties props_1_1 = {};
    props_1_1.pNext = &props_1_2;
    vk::PhysicalDeviceProperties2 props = {};
    props.pNext = &props_1_1;

    device.getProperties2(&props);

    GPULimits limits = {};
    limits.max_image_dimension_1D = props.properties.limits.maxImageDimension1D;
    limits.max_image_dimension_2D = props.properties.limits.maxImageDimension2D;
    limits.max_image_dimension_3D = props.properties.limits.maxImageDimension3D;
    limits.max_image_dimension_cube =
        props.properties.limits.maxImageDimensionCube;
    limits.max_image_array_layers = props.properties.limits.maxImageArrayLayers;
    limits.max_texel_buffer_elements =
        props.properties.limits.maxTexelBufferElements;
    limits.max_uniform_buffer_range =
        props.properties.limits.maxUniformBufferRange;
    limits.max_storage_buffer_range =
        props.properties.limits.maxStorageBufferRange;
    limits.max_push_constants_size = props.properties.limits.maxPushConstantsSize;
    limits.max_memory_allocation_count =
        props.properties.limits.maxMemoryAllocationCount;
    limits.max_sampler_allocation_count =
        props.properties.limits.maxSamplerAllocationCount;
    limits.buffer_image_granularity =
        props.properties.limits.bufferImageGranularity;
    limits.sparse_address_space_size =
        props.properties.limits.sparseAddressSpaceSize;
    limits.max_bound_descriptor_sets =
        props.properties.limits.maxBoundDescriptorSets;
    limits.max_per_stage_descriptor_samplers =
        props.properties.limits.maxPerStageDescriptorSamplers;
    limits.max_per_stage_descriptor_uniform_buffers =
        props.properties.limits.maxPerStageDescriptorUniformBuffers;
    limits.max_per_stage_descriptor_storage_buffers =
        props.properties.limits.maxPerStageDescriptorStorageBuffers;
    limits.max_per_stage_descriptor_sampled_images =
        props.properties.limits.maxPerStageDescriptorSampledImages;
    limits.max_per_stage_descriptor_storage_images =
        props.properties.limits.maxPerStageDescriptorStorageImages;
    limits.max_per_stage_descriptor_input_attachments =
        props.properties.limits.maxPerStageDescriptorInputAttachments;
    limits.max_per_stage_resources = props.properties.limits.maxPerStageResources;
    limits.max_descriptor_set_samplers =
        props.properties.limits.maxDescriptorSetSamplers;
    limits.max_descriptor_set_uniform_buffers =
        props.properties.limits.maxDescriptorSetUniformBuffers;
    limits.max_descriptor_set_uniform_buffers_dynamic =
        props.properties.limits.maxDescriptorSetUniformBuffersDynamic;
    limits.max_descriptor_set_storage_buffers =
        props.properties.limits.maxDescriptorSetStorageBuffers;
    limits.max_descriptor_set_storage_buffers_dynamic =
        props.properties.limits.maxDescriptorSetStorageBuffersDynamic;
    limits.max_descriptor_set_sampled_images =
        props.properties.limits.maxDescriptorSetSampledImages;
    limits.max_descriptor_set_storage_images =
        props.properties.limits.maxDescriptorSetStorageImages;
    limits.max_descriptor_set_input_attachments =
        props.properties.limits.maxDescriptorSetInputAttachments;
    limits.max_vertex_input_attributes =
        props.properties.limits.maxVertexInputAttributes;
    limits.max_vertex_input_bindings =
        props.properties.limits.maxVertexInputBindings;
    limits.max_vertex_input_attribute_offset =
        props.properties.limits.maxVertexInputAttributeOffset;
    limits.max_vertex_input_binding_stride =
        props.properties.limits.maxVertexInputBindingStride;
    limits.max_vertex_output_components =
        props.properties.limits.maxVertexOutputComponents;
    limits.max_tessellation_generation_level =
        props.properties.limits.maxTessellationGenerationLevel;
    limits.max_tessellation_patch_size =
        props.properties.limits.maxTessellationPatchSize;
    limits.max_tessellation_control_per_vertex_input_components =
        props.properties.limits.maxTessellationControlPerVertexInputComponents;
    limits.max_tessellation_control_per_vertex_output_components =
        props.properties.limits.maxTessellationControlPerVertexOutputComponents;
    limits.max_tessellation_control_per_patch_output_components =
        props.properties.limits.maxTessellationControlPerPatchOutputComponents;
    limits.max_tessellation_control_total_output_components =
        props.properties.limits.maxTessellationControlTotalOutputComponents;
    limits.max_tessellation_evaluation_input_components =
        props.properties.limits.maxTessellationEvaluationInputComponents;
    limits.max_tessellation_evaluation_output_components =
        props.properties.limits.maxTessellationEvaluationOutputComponents;
    limits.max_geometry_shader_invocations =
        props.properties.limits.maxGeometryShaderInvocations;
    limits.max_geometry_input_components =
        props.properties.limits.maxGeometryInputComponents;
    limits.max_geometry_output_components =
        props.properties.limits.maxGeometryOutputComponents;
    limits.max_geometry_output_vertices =
        props.properties.limits.maxGeometryOutputVertices;
    limits.max_geometry_total_output_components =
        props.properties.limits.maxGeometryTotalOutputComponents;
    limits.max_fragment_input_components =
        props.properties.limits.maxFragmentInputComponents;
    limits.max_fragment_output_attachments =
        props.properties.limits.maxFragmentOutputAttachments;
    limits.max_fragment_dual_src_attachments =
        props.properties.limits.maxFragmentDualSrcAttachments;
    limits.max_fragment_combined_output_resources =
        props.properties.limits.maxFragmentCombinedOutputResources;
    limits.max_compute_shared_memory_size =
        props.properties.limits.maxComputeSharedMemorySize;
    limits.max_compute_work_group_count =
        props.properties.limits.maxComputeWorkGroupCount;
    limits.max_compute_work_group_invocations =
        props.properties.limits.maxComputeWorkGroupInvocations;
    limits.max_compute_work_group_size =
        props.properties.limits.maxComputeWorkGroupSize;
    limits.sub_pixel_precision_bits =
        props.properties.limits.subPixelPrecisionBits;
    limits.sub_texel_precision_bits =
        props.properties.limits.subTexelPrecisionBits;
    limits.mipmap_precision_bits = props.properties.limits.mipmapPrecisionBits;
    limits.max_draw_indexed_index_value =
        props.properties.limits.maxDrawIndexedIndexValue;
    limits.max_draw_indirect_count = props.properties.limits.maxDrawIndirectCount;
    limits.max_sampler_lod_bias = props.properties.limits.maxSamplerLodBias;
    limits.max_sampler_anisotropy = props.properties.limits.maxSamplerAnisotropy;
    limits.max_viewports = props.properties.limits.maxViewports;
    limits.max_viewport_dimensions =
        props.properties.limits.maxViewportDimensions;
    limits.viewport_bounds_range = props.properties.limits.viewportBoundsRange;
    limits.viewport_sub_pixel_bits = props.properties.limits.viewportSubPixelBits;
    limits.min_memory_map_alignment =
        props.properties.limits.minMemoryMapAlignment;
    limits.min_texel_buffer_offset_alignment =
        props.properties.limits.minTexelBufferOffsetAlignment;
    limits.min_uniform_buffer_offset_alignment =
        props.properties.limits.minUniformBufferOffsetAlignment;
    limits.min_storage_buffer_offset_alignment =
        props.properties.limits.minStorageBufferOffsetAlignment;
    limits.min_texel_offset = props.properties.limits.minTexelOffset;
    limits.max_texel_offset = props.properties.limits.maxTexelOffset;
    limits.min_texel_gather_offset = props.properties.limits.minTexelGatherOffset;
    limits.max_texel_gather_offset = props.properties.limits.maxTexelGatherOffset;
    limits.min_interpolation_offset =
        props.properties.limits.minInterpolationOffset;
    limits.max_interpolation_offset =
        props.properties.limits.maxInterpolationOffset;
    limits.sub_pixel_interpolation_offset_bits =
        props.properties.limits.subPixelInterpolationOffsetBits;
    limits.max_framebuffer_width = props.properties.limits.maxFramebufferWidth;
    limits.max_framebuffer_height = props.properties.limits.maxFramebufferHeight;
    limits.max_framebuffer_layers = props.properties.limits.maxFramebufferLayers;
    limits.framebuffer_color_sample_counts =
        getSampleCountFlags(props.properties.limits.framebufferColorSampleCounts);
    limits.framebuffer_depth_sample_counts =
        getSampleCountFlags(props.properties.limits.framebufferDepthSampleCounts);
    limits.framebuffer_stencil_sample_counts = getSampleCountFlags(
        props.properties.limits.framebufferStencilSampleCounts);
    limits.framebuffer_no_attachments_sample_counts = getSampleCountFlags(
        props.properties.limits.framebufferNoAttachmentsSampleCounts);
    limits.max_color_attachments = props.properties.limits.maxColorAttachments;
    limits.sampled_image_color_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageColorSampleCounts);
    limits.sampled_image_integer_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageIntegerSampleCounts);
    limits.sampled_image_depth_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageDepthSampleCounts);
    limits.sampled_image_stencil_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageStencilSampleCounts);
    limits.storage_image_sample_counts =
        getSampleCountFlags(props.properties.limits.storageImageSampleCounts);
    limits.max_sample_mask_words = props.properties.limits.maxSampleMaskWords;
    limits.timestamp_compute_and_graphics =
        props.properties.limits.timestampComputeAndGraphics;
    limits.timestamp_period = props.properties.limits.timestampPeriod;
    limits.max_clip_distances = props.properties.limits.maxClipDistances;
    limits.max_cull_distances = props.properties.limits.maxCullDistances;
    limits.max_combined_clip_and_cull_distances =
        props.properties.limits.maxCombinedClipAndCullDistances;
    limits.discrete_queue_priorities =
        props.properties.limits.discreteQueuePriorities;
    limits.point_size_range = props.properties.limits.pointSizeRange;
    limits.line_width_range = props.properties.limits.lineWidthRange;
    limits.point_size_granularity = props.properties.limits.pointSizeGranularity;
    limits.line_width_granularity = props.properties.limits.lineWidthGranularity;
    limits.strict_lines = props.properties.limits.strictLines;
    limits.standard_sample_locations =
        props.properties.limits.standardSampleLocations;
    limits.optimal_buffer_copy_offset_alignment =
        props.properties.limits.optimalBufferCopyOffsetAlignment;
    limits.optimal_buffer_copy_row_pitch_alignment =
        props.properties.limits.optimalBufferCopyRowPitchAlignment;
    limits.non_coherent_atom_size = props.properties.limits.nonCoherentAtomSize;
    // Vulkan 1.1 limits
    limits.subgroup_size = props_1_1.subgroupSize;
    limits.subgroup_supported_stages = static_cast<ShaderStageFlags>(static_cast<uint32_t>(props_1_1.
        subgroupSupportedStages));
    limits.subgroup_supported_operations = static_cast<SubgroupFeatureFlags>(static_cast<uint32_t>(props_1_1.
        subgroupSupportedOperations));
    limits.subgroup_quad_operations_in_all_stages = props_1_1.subgroupQuadOperationsInAllStages;
    limits.point_clipping_behavior = static_cast<uint32_t>(props_1_1.pointClippingBehavior);
    limits.max_multiview_view_count = props_1_1.maxMultiviewViewCount;
    limits.max_multiview_instance_index = props_1_1.maxMultiviewInstanceIndex;
    limits.protected_no_fault = props_1_1.protectedNoFault;
    limits.max_per_set_descriptors = props_1_1.maxPerSetDescriptors;
    limits.max_memory_allocation_size = props_1_1.maxMemoryAllocationSize;
    // Vulkan 1.2 Limits
    limits.denorm_behavior_independence = static_cast<uint32_t>(props_1_2.denormBehaviorIndependence);
    limits.rounding_mode_independence = static_cast<uint32_t>(props_1_2.roundingModeIndependence);
    limits.shader_signed_zero_inf_nan_preserve_float_16 = props_1_2.shaderSignedZeroInfNanPreserveFloat16;
    limits.shader_signed_zero_inf_nan_preserve_float_32 = props_1_2.shaderSignedZeroInfNanPreserveFloat32;
    limits.shader_signed_zero_inf_nan_preserve_float_64 = props_1_2.shaderSignedZeroInfNanPreserveFloat64;
    limits.shader_denorm_preserve_float_16 = props_1_2.shaderDenormPreserveFloat16;
    limits.shader_denorm_preserve_float_32 = props_1_2.shaderDenormPreserveFloat32;
    limits.shader_denorm_preserve_float_64 = props_1_2.shaderDenormPreserveFloat64;
    limits.shader_denorm_flush_to_zero_float_16 = props_1_2.shaderDenormFlushToZeroFloat16;
    limits.shader_denorm_flush_to_zero_float_32 = props_1_2.shaderDenormFlushToZeroFloat32;
    limits.shader_denorm_flush_to_zero_float_64 = props_1_2.shaderDenormFlushToZeroFloat64;
    limits.shader_rounding_mode_RTE_float_16 = props_1_2.shaderRoundingModeRTEFloat16;
    limits.shader_rounding_mode_RTE_float_32 = props_1_2.shaderRoundingModeRTEFloat32;
    limits.shader_rounding_mode_RTE_float_64 = props_1_2.shaderRoundingModeRTEFloat64;
    limits.shader_rounding_mode_RTZ_float_16 = props_1_2.shaderRoundingModeRTZFloat16;
    limits.shader_rounding_mode_RTZ_float_32 = props_1_2.shaderRoundingModeRTZFloat32;
    limits.shader_rounding_mode_RTZ_float_64 = props_1_2.shaderRoundingModeRTZFloat64;
    limits.max_update_after_bind_descriptors_in_all_pools = props_1_2.maxUpdateAfterBindDescriptorsInAllPools;
    limits.shader_uniform_buffer_array_non_uniform_indexing_native = props_1_2.
        shaderUniformBufferArrayNonUniformIndexingNative;
    limits.shader_sampled_image_array_non_uniform_indexing_native = props_1_2.
        shaderSampledImageArrayNonUniformIndexingNative;
    limits.shader_storage_buffer_array_non_uniform_indexing_native = props_1_2.
        shaderStorageBufferArrayNonUniformIndexingNative;
    limits.shader_storage_image_array_non_uniform_indexing_native = props_1_2.
        shaderStorageImageArrayNonUniformIndexingNative;
    limits.shader_input_attachment_array_non_uniform_indexing_native = props_1_2.
        shaderInputAttachmentArrayNonUniformIndexingNative;
    limits.robust_buffer_access_update_after_bind = props_1_2.robustBufferAccessUpdateAfterBind;
    limits.quad_divergent_implicit_lod = props_1_2.quadDivergentImplicitLod;
    limits.max_update_after_bind_descriptors_in_all_pools = props_1_2.maxUpdateAfterBindDescriptorsInAllPools;
    limits.shader_uniform_buffer_array_non_uniform_indexing_native = props_1_2.
        shaderUniformBufferArrayNonUniformIndexingNative;
    limits.shader_sampled_image_array_non_uniform_indexing_native = props_1_2.
        shaderSampledImageArrayNonUniformIndexingNative;
    limits.shader_storage_buffer_array_non_uniform_indexing_native = props_1_2.
        shaderStorageBufferArrayNonUniformIndexingNative;
    limits.shader_storage_image_array_non_uniform_indexing_native = props_1_2.
        shaderStorageImageArrayNonUniformIndexingNative;
    limits.shader_input_attachment_array_non_uniform_indexing_native = props_1_2.
        shaderInputAttachmentArrayNonUniformIndexingNative;
    limits.robust_buffer_access_update_after_bind = props_1_2.robustBufferAccessUpdateAfterBind;
    limits.quad_divergent_implicit_lod = props_1_2.quadDivergentImplicitLod;
    limits.max_per_stage_descriptor_update_after_bind_samplers = props_1_2.maxPerStageDescriptorUpdateAfterBindSamplers;
    limits.max_per_stage_descriptor_update_after_bind_uniform_buffers = props_1_2.
        maxPerStageDescriptorUpdateAfterBindUniformBuffers;
    limits.max_per_stage_descriptor_update_after_bind_storage_buffers = props_1_2.
        maxPerStageDescriptorUpdateAfterBindStorageBuffers;
    limits.max_per_stage_descriptor_update_after_bind_sampled_images = props_1_2.
        maxPerStageDescriptorUpdateAfterBindSampledImages;
    limits.max_per_stage_descriptor_update_after_bind_storage_images = props_1_2.
        maxPerStageDescriptorUpdateAfterBindStorageImages;
    limits.max_per_stage_descriptor_update_after_bind_input_attachments = props_1_2.
        maxPerStageDescriptorUpdateAfterBindInputAttachments;
    limits.max_per_stage_update_after_bind_resources = props_1_2.maxPerStageUpdateAfterBindResources;
    limits.max_descriptor_set_update_after_bind_samplers = props_1_2.maxDescriptorSetUpdateAfterBindSamplers;
    limits.max_descriptor_set_update_after_bind_uniform_buffers = props_1_2.
        maxDescriptorSetUpdateAfterBindUniformBuffers;
    limits.max_descriptor_set_update_after_bind_uniform_buffers_dynamic = props_1_2.
        maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
    limits.max_descriptor_set_update_after_bind_storage_buffers = props_1_2.
        maxDescriptorSetUpdateAfterBindStorageBuffers;
    limits.max_descriptor_set_update_after_bind_storage_buffers_dynamic = props_1_2.
        maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
    limits.max_descriptor_set_update_after_bind_sampled_images = props_1_2.maxDescriptorSetUpdateAfterBindSampledImages;
    limits.max_descriptor_set_update_after_bind_storage_images = props_1_2.maxDescriptorSetUpdateAfterBindStorageImages;
    limits.max_descriptor_set_update_after_bind_input_attachments = props_1_2.
        maxDescriptorSetUpdateAfterBindInputAttachments;
    limits.supported_depth_resolve_modes = static_cast<ResolveModeFlags>(static_cast<uint32_t>(props_1_2.
        supportedDepthResolveModes));
    limits.supported_stencil_resolve_modes = static_cast<ResolveModeFlags>(static_cast<uint32_t>(props_1_2.
        supportedStencilResolveModes));
    limits.independent_resolve_none = props_1_2.independentResolveNone;
    limits.independent_resolve = props_1_2.independentResolve;
    limits.filter_minmax_single_component_formats = props_1_2.filterMinmaxSingleComponentFormats;
    limits.filter_minmax_image_component_mapping = props_1_2.filterMinmaxImageComponentMapping;
    limits.max_timeline_semaphore_value_difference = props_1_2.maxTimelineSemaphoreValueDifference;
    limits.framebuffer_integer_color_sample_counts = static_cast<SampleCountFlags>(static_cast<uint32_t>(props_1_2.
        framebufferIntegerColorSampleCounts));
    // Vulkan 1.3 Limits
    limits.min_subgroup_size = props_1_3.minSubgroupSize;
    limits.max_subgroup_size = props_1_3.maxSubgroupSize;
    limits.max_compute_workgroup_subgroups = props_1_3.maxComputeWorkgroupSubgroups;
    limits.required_subgroup_size_stages = static_cast<ShaderStageFlags>(static_cast<uint32_t>(props_1_3.
        requiredSubgroupSizeStages));
    limits.max_inlineUniformBlockSize = props_1_3.maxInlineUniformBlockSize;
    limits.max_per_stage_descriptor_inline_uniform_blocks = props_1_3.maxPerStageDescriptorInlineUniformBlocks;
    limits.max_per_stage_descriptor_update_after_bind_inline_uniform_blocks = props_1_3.
        maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
    limits.max_descriptor_set_inline_uniform_blocks = props_1_3.maxDescriptorSetInlineUniformBlocks;
    limits.max_descriptor_set_update_after_bind_inline_uniform_blocks = props_1_3.
        maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
    limits.max_inline_uniform_total_size = props_1_3.maxInlineUniformTotalSize;
    limits.integer_dot_product_8_bit_unsigned_accelerated = props_1_3.integerDotProduct8BitUnsignedAccelerated;
    limits.integer_dot_product_8_bit_signed_accelerated = props_1_3.integerDotProduct8BitSignedAccelerated;
    limits.integer_dot_product_8_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProduct8BitMixedSignednessAccelerated;
    limits.integer_dot_product_4x8_bit_packed_unsigned_accelerated = props_1_3.
        integerDotProduct4x8BitPackedUnsignedAccelerated;
    limits.integer_dot_product_4x8_bit_packed_signed_accelerated = props_1_3.
        integerDotProduct4x8BitPackedSignedAccelerated;
    limits.integer_dot_product_4x8_bit_packed_mixed_signedness_accelerated = props_1_3.
        integerDotProduct4x8BitPackedMixedSignednessAccelerated;
    limits.integer_dot_product_16_bit_unsigned_accelerated = props_1_3.integerDotProduct16BitUnsignedAccelerated;
    limits.integer_dot_product_16_bit_signed_accelerated = props_1_3.integerDotProduct16BitSignedAccelerated;
    limits.integer_dot_product_16_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProduct16BitMixedSignednessAccelerated;
    limits.integer_dot_product_32_bit_unsigned_accelerated = props_1_3.integerDotProduct32BitUnsignedAccelerated;
    limits.integer_dot_product_32_bit_signed_accelerated = props_1_3.integerDotProduct32BitSignedAccelerated;
    limits.integer_dot_product_32_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProduct32BitMixedSignednessAccelerated;
    limits.integer_dot_product_64_bit_unsigned_accelerated = props_1_3.integerDotProduct64BitUnsignedAccelerated;
    limits.integer_dot_product_64_bit_signed_accelerated = props_1_3.integerDotProduct64BitSignedAccelerated;
    limits.integer_dot_product_64_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProduct64BitMixedSignednessAccelerated;
    limits.integer_dot_product_accumulating_saturating_8_bit_unsigned_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_8_bit_signed_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating8BitSignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_8_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
    limits.integer_dot_product_accumulating_saturating_4x8_bit_packed_unsigned_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_4x8_bit_packed_signed_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_4x8_bit_packed_mixed_signedness_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
    limits.integer_dot_product_accumulating_saturating_16_bit_unsigned_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_16_bit_signed_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating16BitSignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_16_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
    limits.integer_dot_product_accumulating_saturating_32_bit_unsigned_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_32_bit_signed_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating32BitSignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_32_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
    limits.integer_dot_product_accumulating_saturating_64_bit_unsigned_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_64_bit_signed_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating64BitSignedAccelerated;
    limits.integer_dot_product_accumulating_saturating_64_bit_mixed_signedness_accelerated = props_1_3.
        integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
    limits.storage_texel_buffer_offset_alignment_bytes = props_1_3.storageTexelBufferOffsetAlignmentBytes;
    limits.storage_texel_buffer_offset_single_texel_alignment = props_1_3.storageTexelBufferOffsetSingleTexelAlignment;
    limits.uniform_texel_buffer_offset_alignment_bytes = props_1_3.uniformTexelBufferOffsetAlignmentBytes;
    limits.uniform_texel_buffer_offset_single_texel_alignment = props_1_3.uniformTexelBufferOffsetSingleTexelAlignment;
    limits.max_buffer_size = props_1_3.maxBufferSize;
    // Vulkan 1.4 Limits
    limits.line_sub_pixel_precision_bits = props_1_4.lineSubPixelPrecisionBits;
    limits.max_vertex_attrib_divisor = props_1_4.maxVertexAttribDivisor;
    limits.supports_non_zero_first_instance = props_1_4.supportsNonZeroFirstInstance;
    limits.max_push_descriptors = props_1_4.maxPushDescriptors;
    limits.dynamic_rendering_local_read_depth_stencil_attachments = props_1_4.
        dynamicRenderingLocalReadDepthStencilAttachments;
    limits.dynamic_rendering_local_read_multisampled_attachments = props_1_4.
        dynamicRenderingLocalReadMultisampledAttachments;
    limits.early_fragment_multisample_coverage_after_sample_counting = props_1_4.
        earlyFragmentMultisampleCoverageAfterSampleCounting;
    limits.early_fragment_sample_mask_test_before_sample_counting = props_1_4.
        earlyFragmentSampleMaskTestBeforeSampleCounting;
    limits.depth_stencil_swizzle_one_support = props_1_4.depthStencilSwizzleOneSupport;
    limits.polygon_mode_point_size = props_1_4.polygonModePointSize;
    limits.non_strict_single_pixel_wide_lines_use_parallelogram = props_1_4.
        nonStrictSinglePixelWideLinesUseParallelogram;
    limits.non_strict_wide_lines_use_parallelogram = props_1_4.nonStrictWideLinesUseParallelogram;
    limits.block_texel_view_compatible_multiple_layers = props_1_4.blockTexelViewCompatibleMultipleLayers;
    limits.max_combined_image_sampler_descriptor_count = props_1_4.maxCombinedImageSamplerDescriptorCount;
    limits.fragment_shading_rate_clamp_combiner_inputs = props_1_4.fragmentShadingRateClampCombinerInputs;
    limits.default_robustness_storage_buffers = static_cast<uint32_t>(props_1_4.defaultRobustnessStorageBuffers);
    limits.default_robustness_uniform_buffers = static_cast<uint32_t>(props_1_4.defaultRobustnessUniformBuffers);
    limits.default_robustness_vertex_inputs = static_cast<uint32_t>(props_1_4.defaultRobustnessVertexInputs);
    limits.default_robustness_images = static_cast<uint32_t>(props_1_4.defaultRobustnessImages);

    std::vector<vk::ImageLayout> src_layouts(props_1_4.copySrcLayoutCount);
    std::vector<vk::ImageLayout> dst_layouts(props_1_4.copyDstLayoutCount);
    props_1_4.pCopySrcLayouts = src_layouts.data();
    props_1_4.pCopyDstLayouts = dst_layouts.data();
    device.getProperties2(&props);

    const auto convert_layout = [](const std::vector<vk::ImageLayout> &src, std::vector<TextureLayout> &dst) {
        for (size_t i = 0; i < src.size(); ++i)
        {
            dst[i] = getNativeTextureLayout(src[i]);
        }
    };
    limits.copy_src_layouts.resize(props_1_4.copySrcLayoutCount);
    convert_layout(src_layouts, limits.copy_src_layouts);
    limits.copy_dst_layouts.resize(props_1_4.copyDstLayoutCount);
    convert_layout(dst_layouts, limits.copy_dst_layouts);

    limits.optimal_tiling_layout_UUID = props_1_4.optimalTilingLayoutUUID;
    limits.identical_memory_type_requirements = props_1_4.identicalMemoryTypeRequirements;

    // Acceleration Structure limits
    limits.max_geometry_count = accel_struct_props.maxGeometryCount;
    limits.max_instance_count = accel_struct_props.maxInstanceCount;
    limits.max_primitive_count = accel_struct_props.maxPrimitiveCount;
    limits.max_per_stage_descriptor_acceleration_structures =
        accel_struct_props.maxPerStageDescriptorAccelerationStructures;
    limits.max_per_stage_descriptor_update_after_bind_acceleration_structures =
        accel_struct_props
        .maxPerStageDescriptorUpdateAfterBindAccelerationStructures;
    limits.max_descriptor_set_acceleration_structures =
        accel_struct_props.maxDescriptorSetAccelerationStructures;
    limits.max_descriptor_set_update_after_bind_acceleration_structures =
        accel_struct_props.maxDescriptorSetUpdateAfterBindAccelerationStructures;
    limits.min_acceleration_structure_scratch_offset_alignment =
        accel_struct_props.minAccelerationStructureScratchOffsetAlignment;
    // Raytracing limits
    limits.shader_group_handle_size = raytracing_props.shaderGroupHandleSize;
    limits.max_ray_recursion_depth = raytracing_props.maxRayRecursionDepth;
    limits.max_shader_group_stride = raytracing_props.maxShaderGroupStride;
    limits.shader_group_base_alignment =
        raytracing_props.shaderGroupBaseAlignment;
    limits.shader_group_handle_capture_replay_size =
        raytracing_props.shaderGroupHandleCaptureReplaySize;
    limits.max_ray_dispatch_invocation_count =
        raytracing_props.maxRayDispatchInvocationCount;
    limits.shader_group_handle_alignment =
        raytracing_props.shaderGroupHandleAlignment;
    limits.max_ray_hit_attribute_size = raytracing_props.maxRayHitAttributeSize;

    return limits;
}


/**
 * Returns a score based on how many of the features are satisfied.
 * @param f The features of the device.
 * @param p The preferred features used to score.
 * @return A score.
 */
inline uint32_t getGPUFeatureScore(const GPUFeatures &f, const GPUFeatures &p)
{
    uint32_t score = 0;
    // Base Features
    if (p.robust_buffer_access && f.robust_buffer_access == p.robust_buffer_access)
        score += 1;
    if (p.full_draw_index_uint_32 && f.full_draw_index_uint_32 == p.full_draw_index_uint_32)
        score += 1;
    if (p.image_cube_array && f.image_cube_array == p.image_cube_array)
        score += 1;
    if (p.independent_blend && f.independent_blend == p.independent_blend)
        score += 1;
    if (p.geometry_shader && f.geometry_shader == p.geometry_shader)
        score += 1;
    if (p.tessellation_shader && f.tessellation_shader == p.tessellation_shader)
        score += 1;
    if (p.sample_rate_shading && f.sample_rate_shading == p.sample_rate_shading)
        score += 1;
    if (p.dual_src_blend && f.dual_src_blend == p.dual_src_blend)
        score += 1;
    if (p.logic_op && f.logic_op == p.logic_op)
        score += 1;
    if (p.multi_draw_indirect && f.multi_draw_indirect == p.multi_draw_indirect)
        score += 1;
    if (p.draw_indirect_first_instance && f.draw_indirect_first_instance == p.draw_indirect_first_instance)
        score += 1;
    if (p.depth_clamp && f.depth_clamp == p.depth_clamp)
        score += 1;
    if (p.depth_bias_clamp && f.depth_bias_clamp == p.depth_bias_clamp)
        score += 1;
    if (p.fill_mode_non_solid && f.fill_mode_non_solid == p.fill_mode_non_solid)
        score += 1;
    if (p.depth_bounds && f.depth_bounds == p.depth_bounds)
        score += 1;
    if (p.wide_lines && f.wide_lines == p.wide_lines)
        score += 1;
    if (p.large_points && f.large_points == p.large_points)
        score += 1;
    if (p.alpha_to_one && f.alpha_to_one == p.alpha_to_one)
        score += 1;
    if (p.multi_viewport && f.multi_viewport == p.multi_viewport)
        score += 1;
    if (p.sampler_anisotropy && f.sampler_anisotropy == p.sampler_anisotropy)
        score += 1;
    if (p.texture_compression_ETC2 && f.texture_compression_ETC2 == p.texture_compression_ETC2)
        score += 1;
    if (p.texture_compression_ASTCLDR && f.texture_compression_ASTCLDR == p.texture_compression_ASTCLDR)
        score += 1;
    if (p.texture_compression_BC && f.texture_compression_BC == p.texture_compression_BC)
        score += 1;
    if (p.occlusion_query_precise && f.occlusion_query_precise == p.occlusion_query_precise)
        score += 1;
    if (p.pipeline_statistics_query && f.pipeline_statistics_query == p.pipeline_statistics_query)
        score += 1;
    if (p.vertex_pipeline_stores_and_atomics && f.vertex_pipeline_stores_and_atomics == p.
        vertex_pipeline_stores_and_atomics)
        score += 1;
    if (p.fragment_stores_and_atomics && f.fragment_stores_and_atomics == p.fragment_stores_and_atomics)
        score += 1;
    if (p.shader_tessellation_and_geometry_point_size && f.shader_tessellation_and_geometry_point_size == p.
        shader_tessellation_and_geometry_point_size)
        score += 1;
    if (p.shader_image_gather_extended && f.shader_image_gather_extended == p.shader_image_gather_extended)
        score += 1;
    if (p.shader_storage_image_extended_formats && f.shader_storage_image_extended_formats == p.
        shader_storage_image_extended_formats)
        score += 1;
    if (p.shader_storage_image_multisample && f.shader_storage_image_multisample == p.shader_storage_image_multisample)
        score += 1;
    if (p.shader_storage_image_read_without_format && f.shader_storage_image_read_without_format == p.
        shader_storage_image_read_without_format)
        score += 1;
    if (p.shader_storage_image_write_without_format && f.shader_storage_image_write_without_format == p.
        shader_storage_image_write_without_format)
        score += 1;
    if (p.shader_uniform_buffer_array_dynamic_indexing && f.shader_uniform_buffer_array_dynamic_indexing == p.
        shader_uniform_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_sampled_image_array_dynamic_indexing && f.shader_sampled_image_array_dynamic_indexing == p.
        shader_sampled_image_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_buffer_array_dynamic_indexing && f.shader_storage_buffer_array_dynamic_indexing == p.
        shader_storage_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_image_array_dynamic_indexing && f.shader_storage_image_array_dynamic_indexing == p.
        shader_storage_image_array_dynamic_indexing)
        score += 1;
    if (p.shader_clip_distance && f.shader_clip_distance == p.shader_clip_distance)
        score += 1;
    if (p.shader_cull_distance && f.shader_cull_distance == p.shader_cull_distance)
        score += 1;
    if (p.shader_float_64 && f.shader_float_64 == p.shader_float_64)
        score += 1;
    if (p.shader_int_64 && f.shader_int_64 == p.shader_int_64)
        score += 1;
    if (p.shader_int_16 && f.shader_int_16 == p.shader_int_16)
        score += 1;
    if (p.shader_resource_residency && f.shader_resource_residency == p.shader_resource_residency)
        score += 1;
    if (p.shader_resource_min_lod && f.shader_resource_min_lod == p.shader_resource_min_lod)
        score += 1;
    if (p.sparse_binding && f.sparse_binding == p.sparse_binding)
        score += 1;
    if (p.sparse_residency_buffer && f.sparse_residency_buffer == p.sparse_residency_buffer)
        score += 1;
    if (p.sparse_residency_image_2D && f.sparse_residency_image_2D == p.sparse_residency_image_2D)
        score += 1;
    if (p.sparse_residency_image_3D && f.sparse_residency_image_3D == p.sparse_residency_image_3D)
        score += 1;
    if (p.sparse_residency_2_samples && f.sparse_residency_2_samples == p.sparse_residency_2_samples)
        score += 1;
    if (p.sparse_residency_4_samples && f.sparse_residency_4_samples == p.sparse_residency_4_samples)
        score += 1;
    if (p.sparse_residency_8_samples && f.sparse_residency_8_samples == p.sparse_residency_8_samples)
        score += 1;
    if (p.sparse_residency_16_samples && f.sparse_residency_16_samples == p.sparse_residency_16_samples)
        score += 1;
    if (p.sparse_residency_aliased && f.sparse_residency_aliased == p.sparse_residency_aliased)
        score += 1;
    if (p.variable_multisample_rate && f.variable_multisample_rate == p.variable_multisample_rate)
        score += 1;
    if (p.inherited_queries && f.inherited_queries == p.inherited_queries)
        score += 1;
    // Vulkan 1.1 Features
    if (p.storage_buffer_16_bit_access && f.storage_buffer_16_bit_access == p.storage_buffer_16_bit_access)
        score += 1;
    if (p.uniform_and_storage_buffer_16_bit_access && f.uniform_and_storage_buffer_16_bit_access == p.
        uniform_and_storage_buffer_16_bit_access)
        score += 1;
    if (p.storage_push_constant_16 && f.storage_push_constant_16 == p.storage_push_constant_16)
        score += 1;
    if (p.storage_input_output_16 && f.storage_input_output_16 == p.storage_input_output_16)
        score += 1;
    if (p.multiview && f.multiview == p.multiview)
        score += 1;
    if (p.multiview_geometry_shader && f.multiview_geometry_shader == p.multiview_geometry_shader)
        score += 1;
    if (p.multiview_tessellation_shader && f.multiview_tessellation_shader == p.multiview_tessellation_shader)
        score += 1;
    if (p.variable_pointers_storage_buffer && f.variable_pointers_storage_buffer == p.variable_pointers_storage_buffer)
        score += 1;
    if (p.variable_pointers && f.variable_pointers == p.variable_pointers)
        score += 1;
    if (p.protected_memory && f.protected_memory == p.protected_memory)
        score += 1;
    if (p.sampler_YCBCR_conversion && f.sampler_YCBCR_conversion == p.sampler_YCBCR_conversion)
        score += 1;
    if (p.shader_draw_parameters && f.shader_draw_parameters == p.shader_draw_parameters)
        score += 1;
    // Vulkan 1.2 Features
    if (p.sampler_mirror_clamp_to_edge && f.sampler_mirror_clamp_to_edge == p.sampler_mirror_clamp_to_edge)
        score += 1;
    if (p.draw_indirect_count && f.draw_indirect_count == p.draw_indirect_count)
        score += 1;
    if (p.storage_buffer_8_bit_access && f.storage_buffer_8_bit_access == p.storage_buffer_8_bit_access)
        score += 1;
    if (p.uniform_and_storage_buffer_8_bit_access && f.uniform_and_storage_buffer_8_bit_access == p.
        uniform_and_storage_buffer_8_bit_access)
        score += 1;
    if (p.storage_push_constant_8 && f.storage_push_constant_8 == p.storage_push_constant_8)
        score += 1;
    if (p.shader_buffer_int_64_atomics && f.shader_buffer_int_64_atomics == p.shader_buffer_int_64_atomics)
        score += 1;
    if (p.shader_shared_int_64_atomics && f.shader_shared_int_64_atomics == p.shader_shared_int_64_atomics)
        score += 1;
    if (p.shader_float_16 && f.shader_float_16 == p.shader_float_16)
        score += 1;
    if (p.shader_int_8 && f.shader_int_8 == p.shader_int_8)
        score += 1;
    if (p.descriptor_indexing && f.descriptor_indexing == p.descriptor_indexing)
        score += 1;
    if (p.shader_input_attachment_array_dynamic_indexing && f.shader_input_attachment_array_dynamic_indexing == p.
        shader_input_attachment_array_dynamic_indexing)
        score += 1;
    if (p.shader_uniform_texel_buffer_array_dynamic_indexing && f.shader_uniform_texel_buffer_array_dynamic_indexing ==
        p.shader_uniform_texel_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_texel_buffer_array_dynamic_indexing && f.shader_storage_texel_buffer_array_dynamic_indexing ==
        p.shader_storage_texel_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_uniform_buffer_array_non_uniform_indexing && f.shader_uniform_buffer_array_non_uniform_indexing == p.
        shader_uniform_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_sampled_image_array_non_uniform_indexing && f.shader_sampled_image_array_non_uniform_indexing == p.
        shader_sampled_image_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_buffer_array_non_uniform_indexing && f.shader_storage_buffer_array_non_uniform_indexing == p.
        shader_storage_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_image_array_non_uniform_indexing && f.shader_storage_image_array_non_uniform_indexing == p.
        shader_storage_image_array_non_uniform_indexing)
        score += 1;
    if (p.shader_input_attachment_array_non_uniform_indexing && f.shader_input_attachment_array_non_uniform_indexing ==
        p.shader_input_attachment_array_non_uniform_indexing)
        score += 1;
    if (p.shader_uniform_texel_buffer_array_non_uniform_indexing && f.
        shader_uniform_texel_buffer_array_non_uniform_indexing == p.
        shader_uniform_texel_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_texel_buffer_array_non_uniform_indexing && f.
        shader_storage_texel_buffer_array_non_uniform_indexing == p.
        shader_storage_texel_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.descriptor_binding_uniform_buffer_update_after_bind && f.descriptor_binding_uniform_buffer_update_after_bind
        == p.descriptor_binding_uniform_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_sampled_image_update_after_bind && f.descriptor_binding_sampled_image_update_after_bind ==
        p.descriptor_binding_sampled_image_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_image_update_after_bind && f.descriptor_binding_storage_image_update_after_bind ==
        p.descriptor_binding_storage_image_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_buffer_update_after_bind && f.descriptor_binding_storage_buffer_update_after_bind
        == p.descriptor_binding_storage_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_uniform_texel_buffer_update_after_bind && f.
        descriptor_binding_uniform_texel_buffer_update_after_bind == p.
        descriptor_binding_uniform_texel_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_texel_buffer_update_after_bind && f.
        descriptor_binding_storage_texel_buffer_update_after_bind == p.
        descriptor_binding_storage_texel_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_update_unused_while_pending && f.descriptor_binding_update_unused_while_pending == p.
        descriptor_binding_update_unused_while_pending)
        score += 1;
    if (p.descriptor_binding_partially_bound && f.descriptor_binding_partially_bound == p.
        descriptor_binding_partially_bound)
        score += 1;
    if (p.descriptor_binding_variable_descriptor_count && f.descriptor_binding_variable_descriptor_count == p.
        descriptor_binding_variable_descriptor_count)
        score += 1;
    if (p.runtime_descriptor_array && f.runtime_descriptor_array == p.runtime_descriptor_array)
        score += 1;
    // Vulkan 1.3 Features
    if (p.robust_image_access && f.robust_image_access == p.robust_image_access)
        score += 1;
    if (p.inline_uniform_block && f.inline_uniform_block == p.inline_uniform_block)
        score += 1;
    if (p.descriptor_binding_inline_uniform_block_update_after_bind && f.
        descriptor_binding_inline_uniform_block_update_after_bind == p.
        descriptor_binding_inline_uniform_block_update_after_bind)
        score += 1;
    if (p.pipeline_creation_cache_control && f.pipeline_creation_cache_control == p.pipeline_creation_cache_control)
        score += 1;
    if (p.private_data && f.private_data == p.private_data)
        score += 1;
    if (p.shader_demote_to_helper_invocation && f.shader_demote_to_helper_invocation == p.
        shader_demote_to_helper_invocation)
        score += 1;
    if (p.shader_terminate_invocation && f.shader_terminate_invocation == p.shader_terminate_invocation)
        score += 1;
    if (p.subgroup_size_control && f.subgroup_size_control == p.subgroup_size_control)
        score += 1;
    if (p.compute_full_subgroups && f.compute_full_subgroups == p.compute_full_subgroups)
        score += 1;
    if (p.synchronization_2 && f.synchronization_2 == p.synchronization_2)
        score += 1;
    if (p.texture_compression_ASTC_HDR && f.texture_compression_ASTC_HDR == p.texture_compression_ASTC_HDR)
        score += 1;
    if (p.shader_zero_initialize_workgroup_memory && f.shader_zero_initialize_workgroup_memory == p.
        shader_zero_initialize_workgroup_memory)
        score += 1;
    if (p.dynamic_rendering && f.dynamic_rendering == p.dynamic_rendering)
        score += 1;
    if (p.shader_integer_dot_product && f.shader_integer_dot_product == p.shader_integer_dot_product)
        score += 1;
    if (p.maintenance_4 && f.maintenance_4 == p.maintenance_4)
        score += 1;
    // Vulkan 1.4 Features
    if (p.global_priority_query && f.global_priority_query == p.global_priority_query)
        score += 1;
    if (p.shader_subgroup_rotate && f.shader_subgroup_rotate == p.shader_subgroup_rotate)
        score += 1;
    if (p.shader_subgroup_rotate_clustered && f.shader_subgroup_rotate_clustered == p.shader_subgroup_rotate_clustered)
        score += 1;
    if (p.shader_float_controls_2 && f.shader_float_controls_2 == p.shader_float_controls_2)
        score += 1;
    if (p.shader_expect_assume && f.shader_expect_assume == p.shader_expect_assume)
        score += 1;
    if (p.rectangular_lines && f.rectangular_lines == p.rectangular_lines)
        score += 1;
    if (p.bresenham_lines && f.bresenham_lines == p.bresenham_lines)
        score += 1;
    if (p.smooth_lines && f.smooth_lines == p.smooth_lines)
        score += 1;
    if (p.stippled_rectangular_lines && f.stippled_rectangular_lines == p.stippled_rectangular_lines)
        score += 1;
    if (p.stippled_bresenham_lines && f.stippled_bresenham_lines == p.stippled_bresenham_lines)
        score += 1;
    if (p.stippled_smooth_lines && f.stippled_smooth_lines == p.stippled_smooth_lines)
        score += 1;
    if (p.vertex_attribute_instance_rate_divisor && f.vertex_attribute_instance_rate_divisor == p.
        vertex_attribute_instance_rate_divisor)
        score += 1;
    if (p.vertex_attribute_instance_rate_zero_divisor && f.vertex_attribute_instance_rate_zero_divisor == p.
        vertex_attribute_instance_rate_zero_divisor)
        score += 1;
    if (p.index_type_uint_8 && f.index_type_uint_8 == p.index_type_uint_8)
        score += 1;
    if (p.dynamic_rendering_local_read && f.dynamic_rendering_local_read == p.dynamic_rendering_local_read)
        score += 1;
    if (p.maintenance_5 && f.maintenance_5 == p.maintenance_5)
        score += 1;
    if (p.maintenance_6 && f.maintenance_6 == p.maintenance_6)
        score += 1;
    if (p.pipeline_protected_access && f.pipeline_protected_access == p.pipeline_protected_access)
        score += 1;
    if (p.pipeline_robustness && f.pipeline_robustness == p.pipeline_robustness)
        score += 1;
    if (p.host_image_copy && f.host_image_copy == p.host_image_copy)
        score += 1;
    if (p.push_descriptor && f.push_descriptor == p.push_descriptor)
        score += 1;
    // Acceleration Structure features
    if (p.acceleration_structure && f.acceleration_structure == p.acceleration_structure)
        score += 1;
    if (p.acceleration_structure_capture_replay && f.acceleration_structure_capture_replay == p.
        acceleration_structure_capture_replay)
        score += 1;
    if (p.acceleration_structure_indirect_build && f.acceleration_structure_indirect_build == p.
        acceleration_structure_indirect_build)
        score += 1;
    if (p.acceleration_structure_host_commands && f.acceleration_structure_host_commands == p.
        acceleration_structure_host_commands)
        score += 1;
    if (p.descriptor_binding_acceleration_structure_update_after_bind && f.
        descriptor_binding_acceleration_structure_update_after_bind == p.
        descriptor_binding_acceleration_structure_update_after_bind)
        score += 1;
    // Raytracing features
    if (p.ray_tracing_pipeline && f.ray_tracing_pipeline == p.ray_tracing_pipeline)
        score += 1;
    if (p.ray_tracing_pipeline_shader_group_handle_capture_replay && f.
        ray_tracing_pipeline_shader_group_handle_capture_replay == p.
        ray_tracing_pipeline_shader_group_handle_capture_replay)
        score += 1;
    if (p.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed && f.
        ray_tracing_pipeline_shader_group_handle_capture_replay_mixed == p.
        ray_tracing_pipeline_shader_group_handle_capture_replay_mixed)
        score += 1;
    if (p.ray_tracing_pipeline_trace_rays_indirect && f.ray_tracing_pipeline_trace_rays_indirect == p.
        ray_tracing_pipeline_trace_rays_indirect)
        score += 1;
    if (p.ray_traversal_primitive_culling && f.ray_traversal_primitive_culling == p.ray_traversal_primitive_culling)
        score += 1;
    return score;
}

/**
 * Returns a score based on how many of the limits are satisfied.
 * @param l The limits of the device.
 * @param p The preferred limits used to score.
 * @return A score.
 */
inline uint32_t getGPULimitScore(const GPULimits &l, const GPULimits &p)
{
    uint32_t score = 0;
    if (p.max_image_dimension_1D > 0 && l.max_image_dimension_1D >= p.max_image_dimension_1D)
        score += 1;
    if (p.max_image_dimension_2D > 0 && l.max_image_dimension_2D >= p.max_image_dimension_2D)
        score += 1;
    if (p.max_image_dimension_3D > 0 && l.max_image_dimension_3D >= p.max_image_dimension_3D)
        score += 1;
    if (p.max_image_dimension_cube > 0 && l.max_image_dimension_cube >= p.max_image_dimension_cube)
        score += 1;
    if (p.max_image_array_layers > 0 && l.max_image_array_layers >= p.max_image_array_layers)
        score += 1;
    if (p.max_texel_buffer_elements > 0 && l.max_texel_buffer_elements >= p.max_texel_buffer_elements)
        score += 1;
    if (p.max_uniform_buffer_range > 0 && l.max_uniform_buffer_range >= p.max_uniform_buffer_range)
        score += 1;
    if (p.max_storage_buffer_range > 0 && l.max_storage_buffer_range >= p.max_storage_buffer_range)
        score += 1;
    if (p.max_push_constants_size > 0 && l.max_push_constants_size >= p.max_push_constants_size)
        score += 1;
    if (p.max_memory_allocation_count > 0 && l.max_memory_allocation_count >= p.max_memory_allocation_count)
        score
            += 1;
    if (p.max_sampler_allocation_count > 0 && l.max_sampler_allocation_count >= p.max_sampler_allocation_count)
        score += 1;
    if (p.buffer_image_granularity > 0 && l.buffer_image_granularity >= p.buffer_image_granularity)
        score += 1;
    if (p.sparse_address_space_size > 0 && l.sparse_address_space_size >= p.sparse_address_space_size)
        score += 1;
    if (p.max_bound_descriptor_sets > 0 && l.max_bound_descriptor_sets >= p.max_bound_descriptor_sets)
        score += 1;
    if (p.max_per_stage_descriptor_samplers > 0 && l.max_per_stage_descriptor_samplers >= p.
        max_per_stage_descriptor_samplers)
        score += 1;
    if (p.max_per_stage_descriptor_uniform_buffers > 0 && l.max_per_stage_descriptor_uniform_buffers >= p.
        max_per_stage_descriptor_uniform_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_storage_buffers > 0 && l.max_per_stage_descriptor_storage_buffers >= p.
        max_per_stage_descriptor_storage_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_sampled_images > 0 && l.max_per_stage_descriptor_sampled_images >= p.
        max_per_stage_descriptor_sampled_images)
        score += 1;
    if (p.max_per_stage_descriptor_storage_images > 0 && l.max_per_stage_descriptor_storage_images >= p.
        max_per_stage_descriptor_storage_images)
        score += 1;
    if (p.max_per_stage_descriptor_input_attachments > 0 && l.max_per_stage_descriptor_input_attachments >= p.
        max_per_stage_descriptor_input_attachments)
        score += 1;
    if (p.max_per_stage_resources > 0 && l.max_per_stage_resources >= p.max_per_stage_resources)
        score += 1;
    if (p.max_descriptor_set_samplers > 0 && l.max_descriptor_set_samplers >= p.max_descriptor_set_samplers)
        score
            += 1;
    if (p.max_descriptor_set_uniform_buffers > 0 && l.max_descriptor_set_uniform_buffers >= p.
        max_descriptor_set_uniform_buffers)
        score += 1;
    if (p.max_descriptor_set_uniform_buffers_dynamic > 0 && l.max_descriptor_set_uniform_buffers_dynamic >= p.
        max_descriptor_set_uniform_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_storage_buffers > 0 && l.max_descriptor_set_storage_buffers >= p.
        max_descriptor_set_storage_buffers)
        score += 1;
    if (p.max_descriptor_set_storage_buffers_dynamic > 0 && l.max_descriptor_set_storage_buffers_dynamic >= p.
        max_descriptor_set_storage_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_sampled_images > 0 && l.max_descriptor_set_sampled_images >= p.
        max_descriptor_set_sampled_images)
        score += 1;
    if (p.max_descriptor_set_storage_images > 0 && l.max_descriptor_set_storage_images >= p.
        max_descriptor_set_storage_images)
        score += 1;
    if (p.max_descriptor_set_input_attachments > 0 && l.max_descriptor_set_input_attachments >= p.
        max_descriptor_set_input_attachments)
        score += 1;
    if (p.max_vertex_input_attributes > 0 && l.max_vertex_input_attributes >= p.max_vertex_input_attributes)
        score
            += 1;
    if (p.max_vertex_input_bindings > 0 && l.max_vertex_input_bindings >= p.max_vertex_input_bindings)
        score += 1;
    if (p.max_vertex_input_attribute_offset > 0 && l.max_vertex_input_attribute_offset >= p.
        max_vertex_input_attribute_offset)
        score += 1;
    if (p.max_vertex_input_binding_stride > 0 && l.max_vertex_input_binding_stride >= p.
        max_vertex_input_binding_stride)
        score += 1;
    if (p.max_vertex_output_components > 0 && l.max_vertex_output_components >= p.max_vertex_output_components)
        score += 1;
    if (p.max_tessellation_generation_level > 0 && l.max_tessellation_generation_level >= p.
        max_tessellation_generation_level)
        score += 1;
    if (p.max_tessellation_patch_size > 0 && l.max_tessellation_patch_size >= p.max_tessellation_patch_size)
        score
            += 1;
    if (p.max_tessellation_control_per_vertex_input_components > 0 && l.
        max_tessellation_control_per_vertex_input_components >= p.
        max_tessellation_control_per_vertex_input_components)
        score += 1;
    if (p.max_tessellation_control_per_vertex_output_components > 0 && l.
        max_tessellation_control_per_vertex_output_components >= p.
        max_tessellation_control_per_vertex_output_components)
        score += 1;
    if (p.max_tessellation_control_per_patch_output_components > 0 && l.
        max_tessellation_control_per_patch_output_components >= p.
        max_tessellation_control_per_patch_output_components)
        score += 1;
    if (p.max_tessellation_control_total_output_components > 0 && l.max_tessellation_control_total_output_components
        >= p.max_tessellation_control_total_output_components)
        score += 1;
    if (p.max_tessellation_evaluation_input_components > 0 && l.max_tessellation_evaluation_input_components >= p.
        max_tessellation_evaluation_input_components)
        score += 1;
    if (p.max_tessellation_evaluation_output_components > 0 && l.max_tessellation_evaluation_output_components >= p.
        max_tessellation_evaluation_output_components)
        score += 1;
    if (p.max_geometry_shader_invocations > 0 && l.max_geometry_shader_invocations >= p.
        max_geometry_shader_invocations)
        score += 1;
    if (p.max_geometry_input_components > 0 && l.max_geometry_input_components >= p.max_geometry_input_components)
        score += 1;
    if (p.max_geometry_output_components > 0 && l.max_geometry_output_components >= p.
        max_geometry_output_components)
        score += 1;
    if (p.max_geometry_output_vertices > 0 && l.max_geometry_output_vertices >= p.max_geometry_output_vertices)
        score += 1;
    if (p.max_geometry_total_output_components > 0 && l.max_geometry_total_output_components >= p.
        max_geometry_total_output_components)
        score += 1;
    if (p.max_fragment_input_components > 0 && l.max_fragment_input_components >= p.max_fragment_input_components)
        score += 1;
    if (p.max_fragment_output_attachments > 0 && l.max_fragment_output_attachments >= p.
        max_fragment_output_attachments)
        score += 1;
    if (p.max_fragment_dual_src_attachments > 0 && l.max_fragment_dual_src_attachments >= p.
        max_fragment_dual_src_attachments)
        score += 1;
    if (p.max_fragment_combined_output_resources > 0 && l.max_fragment_combined_output_resources >= p.
        max_fragment_combined_output_resources)
        score += 1;
    if (p.max_compute_shared_memory_size > 0 && l.max_compute_shared_memory_size >= p.
        max_compute_shared_memory_size)
        score += 1;
    for (size_t i = 0; i < 3; i++)
    {
        if (p.max_compute_work_group_count[i] > 0 && l.max_compute_work_group_count[i] >= p.
            max_compute_work_group_count[i])
            score += 1;
    }
    if (p.max_compute_work_group_invocations > 0 && l.max_compute_work_group_invocations >= p.
        max_compute_work_group_invocations)
        score += 1;
    for (size_t i = 0; i < 3; i++)
    {
        if (p.max_compute_work_group_size[i] > 0 && l.max_compute_work_group_size[i] >= p.
            max_compute_work_group_size[i])
            score += 1;
    }
    if (p.sub_pixel_precision_bits > 0 && l.sub_pixel_precision_bits >= p.sub_pixel_precision_bits)
        score += 1;
    if (p.sub_texel_precision_bits > 0 && l.sub_texel_precision_bits >= p.sub_texel_precision_bits)
        score += 1;
    if (p.mipmap_precision_bits > 0 && l.mipmap_precision_bits >= p.mipmap_precision_bits)
        score += 1;
    if (p.max_draw_indexed_index_value > 0 && l.max_draw_indexed_index_value >= p.max_draw_indexed_index_value)
        score += 1;
    if (p.max_draw_indirect_count > 0 && l.max_draw_indirect_count >= p.max_draw_indirect_count)
        score += 1;
    if (p.max_sampler_lod_bias != 0.0f && l.max_sampler_lod_bias >= p.max_sampler_lod_bias)
        score += 1;
    if (p.max_sampler_anisotropy != 0.0f && l.max_sampler_anisotropy >= p.max_sampler_anisotropy)
        score += 1;
    if (p.max_viewports > 0 && l.max_viewports >= p.max_viewports)
        score += 1;
    for (size_t i = 0; i < 2; i++)
    {
        if (p.max_viewport_dimensions[i] > 0 && l.max_viewport_dimensions[i] >= p.max_viewport_dimensions[i])
            score
                += 1;
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (p.viewport_bounds_range[i] != 0.0f)
        {
            if ((l.viewport_bounds_range[i] < 0.0f && l.viewport_bounds_range[i] <= p.viewport_bounds_range[i])
                || (l.viewport_bounds_range[i] > 0.0f && l.viewport_bounds_range[i] >= p.viewport_bounds_range[i]))
                score += 1;
        }
    }
    if (p.viewport_sub_pixel_bits > 0 && l.viewport_sub_pixel_bits >= p.viewport_sub_pixel_bits)
        score += 1;
    if (p.min_memory_map_alignment > 0 && l.min_memory_map_alignment >= p.min_memory_map_alignment)
        score += 1;
    if (p.min_texel_buffer_offset_alignment > 0 && l.min_texel_buffer_offset_alignment >= p.
        min_texel_buffer_offset_alignment)
        score += 1;
    if (p.min_uniform_buffer_offset_alignment > 0 && l.min_uniform_buffer_offset_alignment >= p.
        min_uniform_buffer_offset_alignment)
        score += 1;
    if (p.min_storage_buffer_offset_alignment > 0 && l.min_storage_buffer_offset_alignment >= p.
        min_storage_buffer_offset_alignment)
        score += 1;
    if (p.min_texel_offset != 0)
    {
        if ((l.min_texel_offset < 0 && l.min_texel_offset <= p.min_texel_offset)
            || (l.min_texel_offset > 0 && l.min_texel_offset >= p.min_texel_offset))
            score += 1;
    }
    if (p.max_texel_offset > 0 && l.max_texel_offset >= p.max_texel_offset)
        score += 1;
    if (p.min_texel_gather_offset != 0)
    {
        if ((l.min_texel_gather_offset < 0 && l.min_texel_gather_offset <= p.min_texel_gather_offset)
            || (l.min_texel_gather_offset > 0 && l.min_texel_gather_offset >= p.min_texel_gather_offset))
            score += 1;
    }
    if (p.max_texel_gather_offset > 0 && l.max_texel_gather_offset >= p.max_texel_gather_offset)
        score += 1;
    if (p.min_interpolation_offset != 0.0f)
    {
        if ((l.min_interpolation_offset < 0.0f && l.min_interpolation_offset <= p.min_interpolation_offset)
            || (l.min_interpolation_offset > 0.0f && l.min_interpolation_offset >= p.min_interpolation_offset))
            score += 1;
    }
    if (p.max_interpolation_offset != 0.0f)
    {
        if ((l.max_interpolation_offset < 0.0f && l.max_interpolation_offset <= p.max_interpolation_offset)
            || (l.max_interpolation_offset > 0.0f && l.max_interpolation_offset >= p.max_interpolation_offset))
            score += 1;
    }
    if (p.sub_pixel_interpolation_offset_bits > 0 && l.sub_pixel_interpolation_offset_bits >= p.
        sub_pixel_interpolation_offset_bits)
        score += 1;
    if (p.max_framebuffer_width > 0 && l.max_framebuffer_width >= p.max_framebuffer_width)
        score += 1;
    if (p.max_framebuffer_height > 0 && l.max_framebuffer_height >= p.max_framebuffer_height)
        score += 1;
    if (p.max_framebuffer_layers > 0 && l.max_framebuffer_layers >= p.max_framebuffer_layers)
        score += 1;
    if (p.framebuffer_color_sample_counts != SampleCountFlags::NONE && l.framebuffer_color_sample_counts >= p.
        framebuffer_color_sample_counts)
        score += 1;
    if (p.framebuffer_depth_sample_counts != SampleCountFlags::NONE && l.framebuffer_depth_sample_counts >= p.
        framebuffer_depth_sample_counts)
        score += 1;
    if (p.framebuffer_stencil_sample_counts != SampleCountFlags::NONE && l.framebuffer_stencil_sample_counts >= p.
        framebuffer_stencil_sample_counts)
        score += 1;
    if (p.framebuffer_no_attachments_sample_counts != SampleCountFlags::NONE && l.
        framebuffer_no_attachments_sample_counts >= p.framebuffer_no_attachments_sample_counts)
        score += 1;
    if (p.max_color_attachments > 0 && l.max_color_attachments >= p.max_color_attachments)
        score += 1;
    if (p.sampled_image_color_sample_counts != SampleCountFlags::NONE && l.sampled_image_color_sample_counts >= p.
        sampled_image_color_sample_counts)
        score += 1;
    if (p.sampled_image_integer_sample_counts != SampleCountFlags::NONE && l.sampled_image_integer_sample_counts >=
        p.sampled_image_integer_sample_counts)
        score += 1;
    if (p.sampled_image_depth_sample_counts != SampleCountFlags::NONE && l.sampled_image_depth_sample_counts >= p.
        sampled_image_depth_sample_counts)
        score += 1;
    if (p.sampled_image_stencil_sample_counts != SampleCountFlags::NONE && l.sampled_image_stencil_sample_counts >=
        p.sampled_image_stencil_sample_counts)
        score += 1;
    if (p.storage_image_sample_counts != SampleCountFlags::NONE && l.storage_image_sample_counts >= p.
        storage_image_sample_counts)
        score += 1;
    if (p.max_sample_mask_words > 0 && l.max_sample_mask_words >= p.max_sample_mask_words)
        score += 1;
    if (p.timestamp_compute_and_graphics && l.timestamp_compute_and_graphics == p.timestamp_compute_and_graphics)
        score += 1;
    if (p.timestamp_period != 0.0f && l.timestamp_period >= p.timestamp_period)
        score += 1;
    if (p.max_clip_distances > 0 && l.max_clip_distances >= p.max_clip_distances)
        score += 1;
    if (p.max_cull_distances > 0 && l.max_cull_distances >= p.max_cull_distances)
        score += 1;
    if (p.max_combined_clip_and_cull_distances > 0 && l.max_combined_clip_and_cull_distances >= p.
        max_combined_clip_and_cull_distances)
        score += 1;
    if (p.discrete_queue_priorities > 0 && l.discrete_queue_priorities >= p.discrete_queue_priorities)
        score += 1;
    for (size_t i = 0; i < 2; i++)
    {
        if (p.point_size_range[i] > 0.0f && l.point_size_range[i] >= p.point_size_range[i])
            score += 1;
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (p.line_width_range[i] > 0.0f && l.line_width_range[i] >= p.line_width_range[i])
            score += 1;
    }
    if (p.point_size_granularity != 0.0f && l.point_size_granularity >= p.point_size_granularity)
        score += 1;
    if (p.line_width_granularity != 0.0f && l.line_width_granularity >= p.line_width_granularity)
        score += 1;
    if (p.strict_lines && l.strict_lines == p.strict_lines)
        score += 1;
    if (p.standard_sample_locations && l.standard_sample_locations == p.standard_sample_locations)
        score += 1;
    if (p.optimal_buffer_copy_offset_alignment > 0 && l.optimal_buffer_copy_offset_alignment >= p.
        optimal_buffer_copy_offset_alignment)
        score += 1;
    if (p.optimal_buffer_copy_row_pitch_alignment > 0 && l.optimal_buffer_copy_row_pitch_alignment >= p.
        optimal_buffer_copy_row_pitch_alignment)
        score += 1;
    if (p.non_coherent_atom_size > 0 && l.non_coherent_atom_size >= p.non_coherent_atom_size)
        score += 1;
    // Vulkan 1.1 Limits
    if (p.subgroup_size > 0 && l.subgroup_size >= p.subgroup_size)
        score += 1;
    if (p.subgroup_supported_stages != ShaderStageFlags::NONE && l.subgroup_supported_stages >= p.
        subgroup_supported_stages)
        score += 1;
    if (p.subgroup_supported_operations != SubgroupFeatureFlags::NONE && l.subgroup_supported_operations >= p.
        subgroup_supported_operations)
        score += 1;
    if (p.subgroup_quad_operations_in_all_stages && l.subgroup_quad_operations_in_all_stages == p.
        subgroup_quad_operations_in_all_stages)
        score += 1;
    if (p.point_clipping_behavior > 0 && l.point_clipping_behavior >= p.point_clipping_behavior)
        score += 1;
    if (p.max_multiview_view_count > 0 && l.max_multiview_view_count >= p.max_multiview_view_count)
        score += 1;
    if (p.max_multiview_instance_index > 0 && l.max_multiview_instance_index >= p.max_multiview_instance_index)
        score += 1;
    if (p.protected_no_fault && l.protected_no_fault == p.protected_no_fault)
        score += 1;
    if (p.max_per_set_descriptors > 0 && l.max_per_set_descriptors >= p.max_per_set_descriptors)
        score += 1;
    if (p.max_memory_allocation_size > 0 && l.max_memory_allocation_size >= p.max_memory_allocation_size)
        score += 1;
    // Vulkan 1.2 Limits
    if (p.denorm_behavior_independence > 0 && l.denorm_behavior_independence >= p.denorm_behavior_independence)
        score += 1;
    if (p.rounding_mode_independence > 0 && l.rounding_mode_independence >= p.rounding_mode_independence)
        score += 1;
    if (p.shader_signed_zero_inf_nan_preserve_float_16 && l.shader_signed_zero_inf_nan_preserve_float_16 == p.
        shader_signed_zero_inf_nan_preserve_float_16)
        score += 1;
    if (p.shader_signed_zero_inf_nan_preserve_float_32 && l.shader_signed_zero_inf_nan_preserve_float_32 == p.
        shader_signed_zero_inf_nan_preserve_float_32)
        score += 1;
    if (p.shader_signed_zero_inf_nan_preserve_float_64 && l.shader_signed_zero_inf_nan_preserve_float_64 == p.
        shader_signed_zero_inf_nan_preserve_float_64)
        score += 1;
    if (p.shader_denorm_preserve_float_16 && l.shader_denorm_preserve_float_16 == p.shader_denorm_preserve_float_16)
        score += 1;
    if (p.shader_denorm_preserve_float_32 && l.shader_denorm_preserve_float_32 == p.shader_denorm_preserve_float_32)
        score += 1;
    if (p.shader_denorm_preserve_float_64 && l.shader_denorm_preserve_float_64 == p.shader_denorm_preserve_float_64)
        score += 1;
    if (p.shader_denorm_flush_to_zero_float_16 && l.shader_denorm_flush_to_zero_float_16 == p.
        shader_denorm_flush_to_zero_float_16)
        score += 1;
    if (p.shader_denorm_flush_to_zero_float_32 && l.shader_denorm_flush_to_zero_float_32 == p.
        shader_denorm_flush_to_zero_float_32)
        score += 1;
    if (p.shader_denorm_flush_to_zero_float_64 && l.shader_denorm_flush_to_zero_float_64 == p.
        shader_denorm_flush_to_zero_float_64)
        score += 1;
    if (p.shader_rounding_mode_RTE_float_16 && l.shader_rounding_mode_RTE_float_16 == p.
        shader_rounding_mode_RTE_float_16)
        score += 1;
    if (p.shader_rounding_mode_RTE_float_32 && l.shader_rounding_mode_RTE_float_32 == p.
        shader_rounding_mode_RTE_float_32)
        score += 1;
    if (p.shader_rounding_mode_RTE_float_64 && l.shader_rounding_mode_RTE_float_64 == p.
        shader_rounding_mode_RTE_float_64)
        score += 1;
    if (p.shader_rounding_mode_RTZ_float_16 && l.shader_rounding_mode_RTZ_float_16 == p.
        shader_rounding_mode_RTZ_float_16)
        score += 1;
    if (p.shader_rounding_mode_RTZ_float_32 && l.shader_rounding_mode_RTZ_float_32 == p.
        shader_rounding_mode_RTZ_float_32)
        score += 1;
    if (p.shader_rounding_mode_RTZ_float_64 && l.shader_rounding_mode_RTZ_float_64 == p.
        shader_rounding_mode_RTZ_float_64)
        score += 1;
    if (p.max_update_after_bind_descriptors_in_all_pools > 0 && l.max_update_after_bind_descriptors_in_all_pools >= p.
        max_update_after_bind_descriptors_in_all_pools)
        score += 1;
    if (p.shader_uniform_buffer_array_non_uniform_indexing_native && l.
        shader_uniform_buffer_array_non_uniform_indexing_native == p.
        shader_uniform_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_sampled_image_array_non_uniform_indexing_native && l.
        shader_sampled_image_array_non_uniform_indexing_native == p.
        shader_sampled_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_buffer_array_non_uniform_indexing_native && l.
        shader_storage_buffer_array_non_uniform_indexing_native == p.
        shader_storage_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_image_array_non_uniform_indexing_native && l.
        shader_storage_image_array_non_uniform_indexing_native == p.
        shader_storage_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_input_attachment_array_non_uniform_indexing_native && l.
        shader_input_attachment_array_non_uniform_indexing_native == p.
        shader_input_attachment_array_non_uniform_indexing_native)
        score += 1;
    if (p.robust_buffer_access_update_after_bind && l.robust_buffer_access_update_after_bind == p.
        robust_buffer_access_update_after_bind)
        score += 1;
    if (p.quad_divergent_implicit_lod && l.quad_divergent_implicit_lod == p.quad_divergent_implicit_lod)
        score += 1;
    if (p.max_update_after_bind_descriptors_in_all_pools > 0 && l.max_update_after_bind_descriptors_in_all_pools >=
        p.max_update_after_bind_descriptors_in_all_pools)
        score += 1;
    if (p.shader_uniform_buffer_array_non_uniform_indexing_native && l.
        shader_uniform_buffer_array_non_uniform_indexing_native == p.
        shader_uniform_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_sampled_image_array_non_uniform_indexing_native && l.
        shader_sampled_image_array_non_uniform_indexing_native == p.
        shader_sampled_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_buffer_array_non_uniform_indexing_native && l.
        shader_storage_buffer_array_non_uniform_indexing_native == p.
        shader_storage_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_image_array_non_uniform_indexing_native && l.
        shader_storage_image_array_non_uniform_indexing_native == p.
        shader_storage_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_input_attachment_array_non_uniform_indexing_native && l.
        shader_input_attachment_array_non_uniform_indexing_native == p.
        shader_input_attachment_array_non_uniform_indexing_native)
        score += 1;
    if (p.robust_buffer_access_update_after_bind && l.robust_buffer_access_update_after_bind == p.
        robust_buffer_access_update_after_bind)
        score += 1;
    if (p.quad_divergent_implicit_lod && l.quad_divergent_implicit_lod == p.quad_divergent_implicit_lod)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_samplers > 0 && l.
        max_per_stage_descriptor_update_after_bind_samplers >= p.
        max_per_stage_descriptor_update_after_bind_samplers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_uniform_buffers > 0 && l.
        max_per_stage_descriptor_update_after_bind_uniform_buffers >= p.
        max_per_stage_descriptor_update_after_bind_uniform_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_storage_buffers > 0 && l.
        max_per_stage_descriptor_update_after_bind_storage_buffers >= p.
        max_per_stage_descriptor_update_after_bind_storage_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_sampled_images > 0 && l.
        max_per_stage_descriptor_update_after_bind_sampled_images >= p.
        max_per_stage_descriptor_update_after_bind_sampled_images)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_storage_images > 0 && l.
        max_per_stage_descriptor_update_after_bind_storage_images >= p.
        max_per_stage_descriptor_update_after_bind_storage_images)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_input_attachments > 0 && l.
        max_per_stage_descriptor_update_after_bind_input_attachments >= p.
        max_per_stage_descriptor_update_after_bind_input_attachments)
        score += 1;
    if (p.max_per_stage_update_after_bind_resources > 0 && l.max_per_stage_update_after_bind_resources >= p.
        max_per_stage_update_after_bind_resources)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_samplers > 0 && l.max_descriptor_set_update_after_bind_samplers >= p.
        max_descriptor_set_update_after_bind_samplers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_uniform_buffers > 0 && l.
        max_descriptor_set_update_after_bind_uniform_buffers >= p.
        max_descriptor_set_update_after_bind_uniform_buffers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_uniform_buffers_dynamic > 0 && l.
        max_descriptor_set_update_after_bind_uniform_buffers_dynamic >= p.
        max_descriptor_set_update_after_bind_uniform_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_buffers > 0 && l.
        max_descriptor_set_update_after_bind_storage_buffers >= p.
        max_descriptor_set_update_after_bind_storage_buffers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_buffers_dynamic > 0 && l.
        max_descriptor_set_update_after_bind_storage_buffers_dynamic >= p.
        max_descriptor_set_update_after_bind_storage_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_sampled_images > 0 && l.
        max_descriptor_set_update_after_bind_sampled_images >= p.
        max_descriptor_set_update_after_bind_sampled_images)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_images > 0 && l.
        max_descriptor_set_update_after_bind_storage_images >= p.
        max_descriptor_set_update_after_bind_storage_images)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_input_attachments > 0 && l.
        max_descriptor_set_update_after_bind_input_attachments >= p.
        max_descriptor_set_update_after_bind_input_attachments)
        score += 1;
    if (p.supported_depth_resolve_modes != ResolveModeFlags::NONE && l.supported_depth_resolve_modes >= p.
        supported_depth_resolve_modes)
        score += 1;
    if (p.supported_stencil_resolve_modes != ResolveModeFlags::NONE && l.supported_stencil_resolve_modes >= p.
        supported_stencil_resolve_modes)
        score += 1;
    if (p.independent_resolve_none && l.independent_resolve_none == p.independent_resolve_none)
        score += 1;
    if (p.independent_resolve && l.independent_resolve == p.independent_resolve)
        score += 1;
    if (p.filter_minmax_single_component_formats && l.filter_minmax_single_component_formats == p.
        filter_minmax_single_component_formats)
        score += 1;
    if (p.filter_minmax_image_component_mapping && l.filter_minmax_image_component_mapping == p.
        filter_minmax_image_component_mapping)
        score += 1;
    if (p.max_timeline_semaphore_value_difference > 0 && l.max_timeline_semaphore_value_difference >= p.
        max_timeline_semaphore_value_difference)
        score += 1;
    if (p.framebuffer_integer_color_sample_counts != SampleCountFlags::NONE && l.framebuffer_integer_color_sample_counts
        >= p.framebuffer_integer_color_sample_counts)
        score += 1;
    // Vulkan 1.3 Limits
    if (p.min_subgroup_size > 0 && l.min_subgroup_size >= p.min_subgroup_size)
        score += 1;
    if (p.max_subgroup_size > 0 && l.max_subgroup_size >= p.max_subgroup_size)
        score += 1;
    if (p.max_compute_workgroup_subgroups > 0 && l.max_compute_workgroup_subgroups >= p.max_compute_workgroup_subgroups)
        score += 1;
    if (p.required_subgroup_size_stages != ShaderStageFlags::NONE && l.required_subgroup_size_stages >= p.
        required_subgroup_size_stages)
        score += 1;
    if (p.max_inlineUniformBlockSize > 0 && l.max_inlineUniformBlockSize >= p.max_inlineUniformBlockSize)
        score += 1;
    if (p.max_per_stage_descriptor_inline_uniform_blocks > 0 && l.max_per_stage_descriptor_inline_uniform_blocks >= p.
        max_per_stage_descriptor_inline_uniform_blocks)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_inline_uniform_blocks > 0 && l.
        max_per_stage_descriptor_update_after_bind_inline_uniform_blocks >= p.
        max_per_stage_descriptor_update_after_bind_inline_uniform_blocks)
        score += 1;
    if (p.max_descriptor_set_inline_uniform_blocks > 0 && l.max_descriptor_set_inline_uniform_blocks >= p.
        max_descriptor_set_inline_uniform_blocks)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_inline_uniform_blocks > 0 && l.
        max_descriptor_set_update_after_bind_inline_uniform_blocks >= p.
        max_descriptor_set_update_after_bind_inline_uniform_blocks)
        score += 1;
    if (p.max_inline_uniform_total_size > 0 && l.max_inline_uniform_total_size >= p.max_inline_uniform_total_size)
        score += 1;
    if (p.integer_dot_product_8_bit_unsigned_accelerated && l.integer_dot_product_8_bit_unsigned_accelerated == p.
        integer_dot_product_8_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_8_bit_signed_accelerated && l.integer_dot_product_8_bit_signed_accelerated == p.
        integer_dot_product_8_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_8_bit_mixed_signedness_accelerated && l.
        integer_dot_product_8_bit_mixed_signedness_accelerated == p.
        integer_dot_product_8_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_4x8_bit_packed_unsigned_accelerated && l.
        integer_dot_product_4x8_bit_packed_unsigned_accelerated == p.
        integer_dot_product_4x8_bit_packed_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_4x8_bit_packed_signed_accelerated && l.
        integer_dot_product_4x8_bit_packed_signed_accelerated == p.
        integer_dot_product_4x8_bit_packed_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_4x8_bit_packed_mixed_signedness_accelerated && l.
        integer_dot_product_4x8_bit_packed_mixed_signedness_accelerated == p.
        integer_dot_product_4x8_bit_packed_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_16_bit_unsigned_accelerated && l.integer_dot_product_16_bit_unsigned_accelerated == p.
        integer_dot_product_16_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_16_bit_signed_accelerated && l.integer_dot_product_16_bit_signed_accelerated == p.
        integer_dot_product_16_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_16_bit_mixed_signedness_accelerated && l.
        integer_dot_product_16_bit_mixed_signedness_accelerated == p.
        integer_dot_product_16_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_32_bit_unsigned_accelerated && l.integer_dot_product_32_bit_unsigned_accelerated == p.
        integer_dot_product_32_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_32_bit_signed_accelerated && l.integer_dot_product_32_bit_signed_accelerated == p.
        integer_dot_product_32_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_32_bit_mixed_signedness_accelerated && l.
        integer_dot_product_32_bit_mixed_signedness_accelerated == p.
        integer_dot_product_32_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_64_bit_unsigned_accelerated && l.integer_dot_product_64_bit_unsigned_accelerated == p.
        integer_dot_product_64_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_64_bit_signed_accelerated && l.integer_dot_product_64_bit_signed_accelerated == p.
        integer_dot_product_64_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_64_bit_mixed_signedness_accelerated && l.
        integer_dot_product_64_bit_mixed_signedness_accelerated == p.
        integer_dot_product_64_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_8_bit_unsigned_accelerated && l.
        integer_dot_product_accumulating_saturating_8_bit_unsigned_accelerated == p.
        integer_dot_product_accumulating_saturating_8_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_8_bit_signed_accelerated && l.
        integer_dot_product_accumulating_saturating_8_bit_signed_accelerated == p.
        integer_dot_product_accumulating_saturating_8_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_8_bit_mixed_signedness_accelerated && l.
        integer_dot_product_accumulating_saturating_8_bit_mixed_signedness_accelerated == p.
        integer_dot_product_accumulating_saturating_8_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_4x8_bit_packed_unsigned_accelerated && l.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_unsigned_accelerated == p.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_4x8_bit_packed_signed_accelerated && l.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_signed_accelerated == p.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_4x8_bit_packed_mixed_signedness_accelerated && l.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_mixed_signedness_accelerated == p.
        integer_dot_product_accumulating_saturating_4x8_bit_packed_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_16_bit_unsigned_accelerated && l.
        integer_dot_product_accumulating_saturating_16_bit_unsigned_accelerated == p.
        integer_dot_product_accumulating_saturating_16_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_16_bit_signed_accelerated && l.
        integer_dot_product_accumulating_saturating_16_bit_signed_accelerated == p.
        integer_dot_product_accumulating_saturating_16_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_16_bit_mixed_signedness_accelerated && l.
        integer_dot_product_accumulating_saturating_16_bit_mixed_signedness_accelerated == p.
        integer_dot_product_accumulating_saturating_16_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_32_bit_unsigned_accelerated && l.
        integer_dot_product_accumulating_saturating_32_bit_unsigned_accelerated == p.
        integer_dot_product_accumulating_saturating_32_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_32_bit_signed_accelerated && l.
        integer_dot_product_accumulating_saturating_32_bit_signed_accelerated == p.
        integer_dot_product_accumulating_saturating_32_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_32_bit_mixed_signedness_accelerated && l.
        integer_dot_product_accumulating_saturating_32_bit_mixed_signedness_accelerated == p.
        integer_dot_product_accumulating_saturating_32_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_64_bit_unsigned_accelerated && l.
        integer_dot_product_accumulating_saturating_64_bit_unsigned_accelerated == p.
        integer_dot_product_accumulating_saturating_64_bit_unsigned_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_64_bit_signed_accelerated && l.
        integer_dot_product_accumulating_saturating_64_bit_signed_accelerated == p.
        integer_dot_product_accumulating_saturating_64_bit_signed_accelerated)
        score += 1;
    if (p.integer_dot_product_accumulating_saturating_64_bit_mixed_signedness_accelerated && l.
        integer_dot_product_accumulating_saturating_64_bit_mixed_signedness_accelerated == p.
        integer_dot_product_accumulating_saturating_64_bit_mixed_signedness_accelerated)
        score += 1;
    if (p.storage_texel_buffer_offset_alignment_bytes > 0 && l.storage_texel_buffer_offset_alignment_bytes >= p.
        storage_texel_buffer_offset_alignment_bytes)
        score += 1;
    if (p.storage_texel_buffer_offset_single_texel_alignment && l.storage_texel_buffer_offset_single_texel_alignment ==
        p.storage_texel_buffer_offset_single_texel_alignment)
        score += 1;
    if (p.uniform_texel_buffer_offset_alignment_bytes > 0 && l.uniform_texel_buffer_offset_alignment_bytes >= p.
        uniform_texel_buffer_offset_alignment_bytes)
        score += 1;
    if (p.uniform_texel_buffer_offset_single_texel_alignment && l.uniform_texel_buffer_offset_single_texel_alignment ==
        p.uniform_texel_buffer_offset_single_texel_alignment)
        score += 1;
    if (p.max_buffer_size > 0 && l.max_buffer_size >= p.max_buffer_size)
        score += 1;
    // Vulkan 1.4 Limits
    if (p.line_sub_pixel_precision_bits > 0 && l.line_sub_pixel_precision_bits >= p.line_sub_pixel_precision_bits)
        score += 1;
    if (p.max_vertex_attrib_divisor > 0 && l.max_vertex_attrib_divisor >= p.max_vertex_attrib_divisor)
        score += 1;
    if (p.supports_non_zero_first_instance && l.supports_non_zero_first_instance == p.supports_non_zero_first_instance)
        score += 1;
    if (p.max_push_descriptors > 0 && l.max_push_descriptors >= p.max_push_descriptors)
        score += 1;
    if (p.dynamic_rendering_local_read_depth_stencil_attachments && l.
        dynamic_rendering_local_read_depth_stencil_attachments == p.
        dynamic_rendering_local_read_depth_stencil_attachments)
        score += 1;
    if (p.dynamic_rendering_local_read_multisampled_attachments && l.
        dynamic_rendering_local_read_multisampled_attachments == p.
        dynamic_rendering_local_read_multisampled_attachments)
        score += 1;
    if (p.early_fragment_multisample_coverage_after_sample_counting && l.
        early_fragment_multisample_coverage_after_sample_counting == p.
        early_fragment_multisample_coverage_after_sample_counting)
        score += 1;
    if (p.early_fragment_sample_mask_test_before_sample_counting && l.
        early_fragment_sample_mask_test_before_sample_counting == p.
        early_fragment_sample_mask_test_before_sample_counting)
        score += 1;
    if (p.depth_stencil_swizzle_one_support && l.depth_stencil_swizzle_one_support == p.
        depth_stencil_swizzle_one_support)
        score += 1;
    if (p.polygon_mode_point_size && l.polygon_mode_point_size == p.polygon_mode_point_size)
        score += 1;
    if (p.non_strict_single_pixel_wide_lines_use_parallelogram && l.non_strict_single_pixel_wide_lines_use_parallelogram
        == p.non_strict_single_pixel_wide_lines_use_parallelogram)
        score += 1;
    if (p.non_strict_wide_lines_use_parallelogram && l.non_strict_wide_lines_use_parallelogram == p.
        non_strict_wide_lines_use_parallelogram)
        score += 1;
    if (p.block_texel_view_compatible_multiple_layers && l.block_texel_view_compatible_multiple_layers == p.
        block_texel_view_compatible_multiple_layers)
        score += 1;
    if (p.max_combined_image_sampler_descriptor_count > 0 && l.max_combined_image_sampler_descriptor_count >= p.
        max_combined_image_sampler_descriptor_count)
        score += 1;
    if (p.fragment_shading_rate_clamp_combiner_inputs && l.fragment_shading_rate_clamp_combiner_inputs == p.
        fragment_shading_rate_clamp_combiner_inputs)
        score += 1;
    if (p.default_robustness_storage_buffers > 0 && l.default_robustness_storage_buffers >= p.
        default_robustness_storage_buffers)
        score += 1;
    if (p.default_robustness_uniform_buffers > 0 && l.default_robustness_uniform_buffers >= p.
        default_robustness_uniform_buffers)
        score += 1;
    if (p.default_robustness_vertex_inputs > 0 && l.default_robustness_vertex_inputs >= p.
        default_robustness_vertex_inputs)
        score += 1;
    if (p.default_robustness_images > 0 && l.default_robustness_images >= p.default_robustness_images)
        score += 1;
    for (auto p_layout : p.copy_src_layouts)
    {
        for (const auto &l_layout : l.copy_src_layouts)
        {
            if (p_layout == l_layout)
            {
                score += 1;
                break;
            }
        }
    }
    for (auto p_layout : p.copy_dst_layouts)
    {
        for (const auto &l_layout : l.copy_dst_layouts)
        {
            if (p_layout == l_layout)
            {
                score += 1;
                break;
            }
        }
    }
    if (p.identical_memory_type_requirements && l.identical_memory_type_requirements == p.
        identical_memory_type_requirements)
        score += 1;

    // Acceleration Structure limits
    if (p.max_geometry_count > 0 && l.max_geometry_count >= p.max_geometry_count)
        score += 1;
    if (p.max_instance_count > 0 && l.max_instance_count >= p.max_instance_count)
        score += 1;
    if (p.max_primitive_count > 0 && l.max_primitive_count >= p.max_primitive_count)
        score += 1;
    if (p.max_per_stage_descriptor_acceleration_structures > 0 && l.max_per_stage_descriptor_acceleration_structures
        >= p.max_per_stage_descriptor_acceleration_structures)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_acceleration_structures > 0 && l.
        max_per_stage_descriptor_update_after_bind_acceleration_structures >= p.
        max_per_stage_descriptor_update_after_bind_acceleration_structures)
        score += 1;
    if (p.max_descriptor_set_acceleration_structures > 0 && l.max_descriptor_set_acceleration_structures >= p.
        max_descriptor_set_acceleration_structures)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_acceleration_structures > 0 && l.
        max_descriptor_set_update_after_bind_acceleration_structures >= p.
        max_descriptor_set_update_after_bind_acceleration_structures)
        score += 1;
    if (p.min_acceleration_structure_scratch_offset_alignment > 0 && l.
        min_acceleration_structure_scratch_offset_alignment >= p.
        min_acceleration_structure_scratch_offset_alignment)
        score += 1;
    // Raytracing limits
    if (p.shader_group_handle_size > 0 && l.shader_group_handle_size >= p.shader_group_handle_size)
        score += 1;
    if (p.max_ray_recursion_depth > 0 && l.max_ray_recursion_depth >= p.max_ray_recursion_depth)
        score += 1;
    if (p.max_shader_group_stride > 0 && l.max_shader_group_stride >= p.max_shader_group_stride)
        score += 1;
    if (p.shader_group_base_alignment > 0 && l.shader_group_base_alignment >= p.shader_group_base_alignment)
        score += 1;
    if (p.shader_group_handle_capture_replay_size > 0 && l.shader_group_handle_capture_replay_size >= p.
        shader_group_handle_capture_replay_size)
        score += 1;
    if (p.max_ray_dispatch_invocation_count > 0 && l.max_ray_dispatch_invocation_count >= p.
        max_ray_dispatch_invocation_count)
        score += 1;
    if (p.shader_group_handle_alignment > 0 && l.shader_group_handle_alignment >= p.shader_group_handle_alignment)
        score += 1;
    if (p.max_ray_hit_attribute_size > 0 && l.max_ray_hit_attribute_size >= p.max_ray_hit_attribute_size)
        score += 1;
    return score;
}

/**
 * Returns a score for the given physical device based on the specified GPU selection preference.
 * @param device Vulkan Physical Device handle.
 * @param surface Vulkan Surface handle. (Pass `nullptr` if there's none).
 * @param preference GPU type, features, limits, etc. used to score the given device.
 * @return integer number indicating the score for the device.
 */
inline uint32_t getGPUScore(const vk::PhysicalDevice device, const vk::SurfaceKHR surface,
                            const GPUSelectionPreference &preference)
{
    uint32_t score = 0;
    if (preference.type != GPUType::UNKNOWN &&
        preference.type == getGPUType(device.getProperties().deviceType))
    {
        score += 1000;
    }

    const GPUFeatures features = getGPUFeatures(device);
    score += getGPUFeatureScore(features, preference.features);
    const GPULimits limits = getGPULimits(device);
    score += getGPULimitScore(limits, preference.limits);

    std::vector<QueueFamily> queue_families = getQueueFamilies(device, surface);
    auto all_family_flags = QueueFamilyFlags::NONE;
    bool supports_async_compute = false;
    bool supports_async_transfer = false;
    for (const auto &q_f : queue_families)
    {
        if (q_f.supportsOnlyCompute())
        {
            supports_async_compute = true;
        }
        if (q_f.supportsOnlyTransfer())
        {
            supports_async_transfer = true;
        }
        all_family_flags = all_family_flags | q_f.support_flags;
    }

    // Check if all the preferred queue types are present in the device.
    if (preference.queue_types != QueueFamilyFlags::NONE && (all_family_flags & preference.queue_types) ==
        preference.queue_types)
    {
        score += 1000;
    }
    // If there are multiple queue families, check if there's a dedicated compute queue family.
    if (preference.dedicated_compute_queue && supports_async_compute)
    {
        score += 1000;
    }
    // If there are multiple queue families, check if there's a dedicated transfer queue family.
    if (preference.dedicated_transfer_queue && supports_async_transfer)
    {
        score += 1000;
    }

    return score;
}

struct EnabledFeatures
{
    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_features = {};
    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accel_struct_features = {};
    vk::PhysicalDeviceVulkan14Features base_1_4 = {};
    vk::PhysicalDeviceVulkan13Features base_1_3 = {};
    vk::PhysicalDeviceVulkan12Features base_1_2 = {};
    vk::PhysicalDeviceVulkan11Features base_1_1 = {};
    vk::PhysicalDeviceFeatures2 base = {};
};

/**
 * Returns physical device features which are used when enabling features when creating a
 * logical device.
 * @param f The features to be enabled on logical device creation.
 * @return A Vulkan physical device features.
 */
inline void getEnabledFeatures(const GPUFeatures &f, EnabledFeatures *const out_enabled_features)
{
    auto &features = out_enabled_features->base;
    // Base Features
    if (f.robust_buffer_access)
        features.features.robustBufferAccess = true;
    if (f.full_draw_index_uint_32)
        features.features.fullDrawIndexUint32 = true;
    if (f.image_cube_array)
        features.features.imageCubeArray = true;
    if (f.independent_blend)
        features.features.independentBlend = true;
    if (f.geometry_shader)
        features.features.geometryShader = true;
    if (f.tessellation_shader)
        features.features.tessellationShader = true;
    if (f.sample_rate_shading)
        features.features.sampleRateShading = true;
    if (f.dual_src_blend)
        features.features.dualSrcBlend = true;
    if (f.logic_op)
        features.features.logicOp = true;
    if (f.multi_draw_indirect)
        features.features.multiDrawIndirect = true;
    if (f.draw_indirect_first_instance)
        features.features.drawIndirectFirstInstance = true;
    if (f.depth_clamp)
        features.features.depthClamp = true;
    if (f.depth_bias_clamp)
        features.features.depthBiasClamp = true;
    if (f.fill_mode_non_solid)
        features.features.fillModeNonSolid = true;
    if (f.depth_bounds)
        features.features.depthBounds = true;
    if (f.wide_lines)
        features.features.wideLines = true;
    if (f.large_points)
        features.features.largePoints = true;
    if (f.alpha_to_one)
        features.features.alphaToOne = true;
    if (f.multi_viewport)
        features.features.multiViewport = true;
    if (f.sampler_anisotropy)
        features.features.samplerAnisotropy = true;
    if (f.texture_compression_ETC2)
        features.features.textureCompressionETC2 = true;
    if (f.texture_compression_ASTCLDR)
        features.features.textureCompressionASTC_LDR = true;
    if (f.texture_compression_BC)
        features.features.textureCompressionBC = true;
    if (f.occlusion_query_precise)
        features.features.occlusionQueryPrecise = true;
    if (f.pipeline_statistics_query)
        features.features.pipelineStatisticsQuery = true;
    if (f.vertex_pipeline_stores_and_atomics)
        features.features.vertexPipelineStoresAndAtomics = true;
    if (f.fragment_stores_and_atomics)
        features.features.fragmentStoresAndAtomics = true;
    if (f.shader_tessellation_and_geometry_point_size)
        features.features.shaderTessellationAndGeometryPointSize =
            true;
    if (f.shader_image_gather_extended)
        features.features.shaderImageGatherExtended = true;
    if (f.shader_storage_image_extended_formats)
        features.features.shaderStorageImageExtendedFormats = true;
    if (f.shader_storage_image_multisample)
        features.features.shaderStorageImageMultisample = true;
    if (f.shader_storage_image_read_without_format)
        features.features.shaderStorageImageReadWithoutFormat = true;
    if (f.shader_storage_image_write_without_format)
        features.features.shaderStorageImageWriteWithoutFormat = true;
    if (f.shader_uniform_buffer_array_dynamic_indexing)
        features.features.shaderUniformBufferArrayDynamicIndexing =
            true;
    if (f.shader_sampled_image_array_dynamic_indexing)
        features.features.shaderSampledImageArrayDynamicIndexing =
            true;
    if (f.shader_storage_buffer_array_dynamic_indexing)
        features.features.shaderStorageBufferArrayDynamicIndexing =
            true;
    if (f.shader_storage_image_array_dynamic_indexing)
        features.features.shaderStorageImageArrayDynamicIndexing =
            true;
    if (f.shader_clip_distance)
        features.features.shaderClipDistance = true;
    if (f.shader_cull_distance)
        features.features.shaderCullDistance = true;
    if (f.shader_float_64)
        features.features.shaderFloat64 = true;
    if (f.shader_int_64)
        features.features.shaderInt64 = true;
    if (f.shader_int_16)
        features.features.shaderInt16 = true;
    if (f.shader_resource_residency)
        features.features.shaderResourceResidency = true;
    if (f.shader_resource_min_lod)
        features.features.shaderResourceMinLod = true;
    if (f.sparse_binding)
        features.features.sparseBinding = true;
    if (f.sparse_residency_buffer)
        features.features.sparseResidencyBuffer = true;
    if (f.sparse_residency_image_2D)
        features.features.sparseResidencyImage2D = true;
    if (f.sparse_residency_image_3D)
        features.features.sparseResidencyImage3D = true;
    if (f.sparse_residency_2_samples)
        features.features.sparseResidency2Samples = true;
    if (f.sparse_residency_4_samples)
        features.features.sparseResidency4Samples = true;
    if (f.sparse_residency_8_samples)
        features.features.sparseResidency8Samples = true;
    if (f.sparse_residency_16_samples)
        features.features.sparseResidency16Samples = true;
    if (f.sparse_residency_aliased)
        features.features.sparseResidencyAliased = true;
    if (f.variable_multisample_rate)
        features.features.variableMultisampleRate = true;
    if (f.inherited_queries)
        features.features.inheritedQueries = true;
    // Vulkan 1.1 Features
    auto &features_1_1 = out_enabled_features->base_1_1;
    if (f.storage_buffer_16_bit_access)
        features_1_1.storageBuffer16BitAccess = true;
    if (f.uniform_and_storage_buffer_16_bit_access)
        features_1_1.uniformAndStorageBuffer16BitAccess = true;
    if (f.storage_push_constant_16)
        features_1_1.storagePushConstant16 = true;
    if (f.storage_input_output_16)
        features_1_1.storageInputOutput16 = true;
    if (f.multiview)
        features_1_1.multiview = true;
    if (f.multiview_geometry_shader)
        features_1_1.multiviewGeometryShader = true;
    if (f.multiview_tessellation_shader)
        features_1_1.multiviewTessellationShader = true;
    if (f.variable_pointers_storage_buffer)
        features_1_1.variablePointersStorageBuffer = true;
    if (f.variable_pointers)
        features_1_1.variablePointers = true;
    if (f.protected_memory)
        features_1_1.protectedMemory = true;
    if (f.sampler_YCBCR_conversion)
        features_1_1.samplerYcbcrConversion = true;
    if (f.shader_draw_parameters)
        features_1_1.shaderDrawParameters = true;
    // Vulkan 1.2 Features
    auto &features_1_2 = out_enabled_features->base_1_2;
    if (f.sampler_mirror_clamp_to_edge)
        features_1_2.samplerMirrorClampToEdge = true;
    if (f.draw_indirect_count)
        features_1_2.drawIndirectCount = true;
    if (f.storage_buffer_8_bit_access)
        features_1_2.storageBuffer8BitAccess = true;
    if (f.uniform_and_storage_buffer_8_bit_access)
        features_1_2.uniformAndStorageBuffer8BitAccess = true;
    if (f.storage_push_constant_8)
        features_1_2.storagePushConstant8 = true;
    if (f.shader_buffer_int_64_atomics)
        features_1_2.shaderBufferInt64Atomics = true;
    if (f.shader_shared_int_64_atomics)
        features_1_2.shaderSharedInt64Atomics = true;
    if (f.shader_float_16)
        features_1_2.shaderFloat16 = true;
    if (f.shader_int_8)
        features_1_2.shaderInt8 = true;
    if (f.descriptor_indexing)
        features_1_2.descriptorIndexing = true;
    if (f.shader_input_attachment_array_dynamic_indexing)
        features_1_2.
            shaderInputAttachmentArrayDynamicIndexing = true;
    if (f.shader_uniform_texel_buffer_array_dynamic_indexing)
        features_1_2.shaderUniformTexelBufferArrayDynamicIndexing = true;
    if (f.shader_storage_texel_buffer_array_dynamic_indexing)
        features_1_2.
            shaderStorageTexelBufferArrayNonUniformIndexing = true;
    if (f.shader_uniform_buffer_array_non_uniform_indexing)
        features_1_2.
            shaderUniformBufferArrayNonUniformIndexing = true;
    if (f.shader_sampled_image_array_non_uniform_indexing)
        features_1_2.
            shaderSampledImageArrayNonUniformIndexing = true;
    if (f.shader_storage_buffer_array_non_uniform_indexing)
        features_1_2.
            shaderStorageBufferArrayNonUniformIndexing = true;
    if (f.shader_storage_image_array_non_uniform_indexing)
        features_1_2.
            shaderStorageImageArrayNonUniformIndexing = true;
    if (f.shader_input_attachment_array_non_uniform_indexing)
        features_1_2.
            shaderInputAttachmentArrayNonUniformIndexing = true;
    if (f.shader_uniform_texel_buffer_array_non_uniform_indexing)
        features_1_2.
            shaderUniformTexelBufferArrayNonUniformIndexing = true;
    if (f.shader_storage_texel_buffer_array_non_uniform_indexing)
        features_1_2.
            shaderStorageTexelBufferArrayNonUniformIndexing = true;
    if (f.descriptor_binding_uniform_buffer_update_after_bind)
        features_1_2.
            descriptorBindingUniformBufferUpdateAfterBind = true;
    if (f.descriptor_binding_sampled_image_update_after_bind)
        features_1_2.
            descriptorBindingSampledImageUpdateAfterBind = true;
    if (f.descriptor_binding_storage_image_update_after_bind)
        features_1_2.
            descriptorBindingStorageImageUpdateAfterBind = true;
    if (f.descriptor_binding_storage_buffer_update_after_bind)
        features_1_2.
            descriptorBindingStorageBufferUpdateAfterBind = true;
    if (f.descriptor_binding_uniform_texel_buffer_update_after_bind)
        features_1_2.
            descriptorBindingUniformTexelBufferUpdateAfterBind = true;
    if (f.descriptor_binding_storage_texel_buffer_update_after_bind)
        features_1_2.
            descriptorBindingStorageTexelBufferUpdateAfterBind = true;
    if (f.descriptor_binding_update_unused_while_pending)
        features_1_2.
            descriptorBindingUpdateUnusedWhilePending = true;
    if (f.descriptor_binding_partially_bound)
        features_1_2.descriptorBindingPartiallyBound = true;
    if (f.descriptor_binding_variable_descriptor_count)
        features_1_2
            .descriptorBindingVariableDescriptorCount = true;
    if (f.runtime_descriptor_array)
        features_1_2.runtimeDescriptorArray = true;
    if (f.sampler_filter_minmax)
        features_1_2.samplerFilterMinmax = true;
    if (f.scalar_block_layout)
        features_1_2.scalarBlockLayout = true;
    if (f.imageless_framebuffer)
        features_1_2.imagelessFramebuffer = true;
    if (f.uniform_buffer_standard_layout)
        features_1_2.uniformBufferStandardLayout = true;
    if (f.shader_subgroup_extended_types)
        features_1_2.shaderSubgroupExtendedTypes = true;
    if (f.separate_depth_stencil_layouts)
        features_1_2.separateDepthStencilLayouts = true;
    if (f.host_query_reset)
        features_1_2.hostQueryReset = true;
    if (f.timeline_semaphore)
        features_1_2.timelineSemaphore = true;
    if (f.buffer_device_address)
        features_1_2.bufferDeviceAddress = true;
    if (f.buffer_device_address_capture_replay)
        features_1_2.bufferDeviceAddressCaptureReplay = true;
    if (f.buffer_device_address_multi_device)
        features_1_2.bufferDeviceAddressMultiDevice = true;
    if (f.vulkan_memory_model)
        features_1_2.vulkanMemoryModel = true;
    if (f.vulkan_memory_model_device_scope)
        features_1_2.vulkanMemoryModelDeviceScope = true;
    if (f.vulkan_memory_model_availability_visibility_chains)
        features_1_2.vulkanMemoryModelAvailabilityVisibilityChains = true;
    if (f.shader_output_viewport_index)
        features_1_2.shaderOutputViewportIndex = true;
    if (f.shader_output_layer)
        features_1_2.shaderOutputLayer = true;
    if (f.subgroup_broadcast_dynamic_id)
        features_1_2.subgroupBroadcastDynamicId = true;
    // Vulkan 1.3 Features
    auto &features_1_3 = out_enabled_features->base_1_3;
    if (f.robust_image_access)
        features_1_3.robustImageAccess = true;
    if (f.inline_uniform_block)
        features_1_3.inlineUniformBlock = true;
    if (f.descriptor_binding_inline_uniform_block_update_after_bind)
        features_1_3.descriptorBindingInlineUniformBlockUpdateAfterBind = true;
    if (f.pipeline_creation_cache_control)
        features_1_3.pipelineCreationCacheControl = true;
    if (f.private_data)
        features_1_3.privateData = true;
    if (f.shader_demote_to_helper_invocation)
        features_1_3.shaderDemoteToHelperInvocation = true;
    if (f.shader_terminate_invocation)
        features_1_3.shaderTerminateInvocation = true;
    if (f.subgroup_size_control)
        features_1_3.subgroupSizeControl = true;
    if (f.compute_full_subgroups)
        features_1_3.computeFullSubgroups = true;
    if (f.synchronization_2)
        features_1_3.synchronization2 = true;
    if (f.texture_compression_ASTC_HDR)
        features_1_3.textureCompressionASTC_HDR = true;
    if (f.shader_zero_initialize_workgroup_memory)
        features_1_3.shaderZeroInitializeWorkgroupMemory = true;
    if (f.dynamic_rendering)
        features_1_3.dynamicRendering = true;
    if (f.shader_integer_dot_product)
        features_1_3.shaderIntegerDotProduct = true;
    if (f.maintenance_4)
        features_1_3.maintenance4 = true;
    // Vulkan 1.4 Features
    auto &features_1_4 = out_enabled_features->base_1_4;
    if (f.global_priority_query)
        features_1_4.globalPriorityQuery = true;
    if (f.shader_subgroup_rotate)
        features_1_4.shaderSubgroupRotate = true;
    if (f.shader_subgroup_rotate_clustered)
        features_1_4.shaderSubgroupRotateClustered = true;
    if (f.shader_float_controls_2)
        features_1_4.shaderFloatControls2 = true;
    if (f.shader_expect_assume)
        features_1_4.shaderExpectAssume = true;
    if (f.rectangular_lines)
        features_1_4.rectangularLines = true;
    if (f.bresenham_lines)
        features_1_4.bresenhamLines = true;
    if (f.smooth_lines)
        features_1_4.smoothLines = true;
    if (f.stippled_rectangular_lines)
        features_1_4.stippledRectangularLines = true;
    if (f.stippled_bresenham_lines)
        features_1_4.stippledBresenhamLines = true;
    if (f.stippled_smooth_lines)
        features_1_4.stippledSmoothLines = true;
    if (f.vertex_attribute_instance_rate_divisor)
        features_1_4.vertexAttributeInstanceRateDivisor = true;
    if (f.vertex_attribute_instance_rate_zero_divisor)
        features_1_4.vertexAttributeInstanceRateZeroDivisor = true;
    if (f.index_type_uint_8)
        features_1_4.indexTypeUint8 = true;
    if (f.dynamic_rendering_local_read)
        features_1_4.dynamicRenderingLocalRead = true;
    if (f.maintenance_5)
        features_1_4.maintenance5 = true;
    if (f.maintenance_6)
        features_1_4.maintenance6 = true;
    if (f.pipeline_protected_access)
        features_1_4.pipelineProtectedAccess = true;
    if (f.pipeline_robustness)
        features_1_4.pipelineRobustness = true;
    if (f.host_image_copy)
        features_1_4.hostImageCopy = true;
    if (f.push_descriptor)
        features_1_4.pushDescriptor = true;
    // Acceleration Structure features
    auto &accel_struct_features = out_enabled_features->accel_struct_features;
    if (f.acceleration_structure)
        accel_struct_features.accelerationStructure = true;
    if (f.acceleration_structure_capture_replay)
        accel_struct_features.accelerationStructureCaptureReplay = true;
    if (f.acceleration_structure_indirect_build)
        accel_struct_features.accelerationStructureIndirectBuild = true;
    if (f.acceleration_structure_host_commands)
        accel_struct_features.accelerationStructureHostCommands = true;
    if (f.descriptor_binding_acceleration_structure_update_after_bind)
        accel_struct_features.
            descriptorBindingAccelerationStructureUpdateAfterBind = true;
    // Raytracing features
    auto &raytracing_features = out_enabled_features->raytracing_features;
    if (f.ray_tracing_pipeline)
        raytracing_features.rayTracingPipeline = true;
    if (f.ray_tracing_pipeline_shader_group_handle_capture_replay)
        raytracing_features.
            rayTracingPipelineShaderGroupHandleCaptureReplay = true;
    if (f.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed)
        raytracing_features.
            rayTracingPipelineShaderGroupHandleCaptureReplayMixed = true;
    if (f.ray_tracing_pipeline_trace_rays_indirect)
        raytracing_features.rayTracingPipelineTraceRaysIndirect = true;
    if (f.ray_traversal_primitive_culling)
        raytracing_features.rayTraversalPrimitiveCulling = true;

    accel_struct_features.pNext = &raytracing_features;
    features_1_4.pNext = &accel_struct_features;
    features_1_3.pNext = &features_1_4;
    features_1_2.pNext = &features_1_3;
    features_1_1.pNext = &features_1_2;
    features.pNext = &features_1_1;
}

/**
 * Returns device extension names that need to be enabled to support given features.
 * @param f The features to be enabled on logical device creation.
 * @return A vector of device extension names.
 */
inline std::vector<const char *> getEnabledDeviceExtensions(const GPUFeatures &f)
{
    std::vector<const char *> extensions = {};
    // Acceleration Structure features
    if (f.acceleration_structure
        || f.acceleration_structure_capture_replay
        || f.acceleration_structure_indirect_build
        || f.acceleration_structure_host_commands
        || f.descriptor_binding_acceleration_structure_update_after_bind)
    {
        extensions.push_back(vk::KHRAccelerationStructureExtensionName);
    }
    // Raytracing features
    if (f.ray_tracing_pipeline
        || f.ray_tracing_pipeline_shader_group_handle_capture_replay
        || f.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed
        || f.ray_tracing_pipeline_trace_rays_indirect
        || f.ray_traversal_primitive_culling)
    {
        extensions.push_back(vk::KHRRayTracingPipelineExtensionName);
    }
    return extensions;
}

inline void transitionImageLayout(const vk::CommandBuffer &cmd_buffer, const vk::Image image, const vk::Format format,
                                  const vk::ImageLayout old_layout, const vk::ImageLayout new_layout)
{
    const vk::ImageAspectFlags image_aspect = isDepthTextureFormat(format)
                                                  ? vk::ImageAspectFlagBits::eDepth
                                                  : vk::ImageAspectFlagBits::eColor;
    const auto subresource = vk::ImageSubresourceRange{image_aspect, 0, vk::RemainingMipLevels, 0,
                                                       vk::RemainingArrayLayers};
    const auto barrier = vk::ImageMemoryBarrier2{vk::PipelineStageFlagBits2::eAllCommands,
                                                 vk::AccessFlagBits2::eMemoryWrite,
                                                 vk::PipelineStageFlagBits2::eAllCommands,
                                                 vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
                                                 old_layout, new_layout,
                                                 vk::QueueFamilyIgnored, vk::QueueFamilyIgnored,
                                                 image, subresource};
    vk::DependencyInfo dep_info = {};
    dep_info.setImageMemoryBarriers({barrier});
    cmd_buffer.pipelineBarrier2(dep_info);
}
}
#endif // KIRANA_RENDERER_HELPERS_VULKAN_HPP