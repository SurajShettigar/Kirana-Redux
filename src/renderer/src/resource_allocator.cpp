// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#define VMA_IMPLEMENTATION
#include "resource_allocator.hpp"

#include "helpers_vulkan.hpp"
#include "device.hpp"

#include <logger.hpp>

namespace kirana::renderer
{

inline VmaMemoryUsage getMemoryUsage(const ResourceMemoryType memory_type)
{
    switch (memory_type)
    {
    case ResourceMemoryType::AUTO:
        return VMA_MEMORY_USAGE_AUTO;
    case ResourceMemoryType::DEVICE:
        return VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    case ResourceMemoryType::HOST:
        return VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    default:
        return VMA_MEMORY_USAGE_UNKNOWN;
    }
}

bool ResourceAllocator::init(const Device *device)
{
    m_device = device;
    if (!m_device)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to initialize resource allocator: Invalid reference to device.");

        return false;
    }
    VmaAllocatorCreateInfo create_info = {};
    create_info.instance = m_device->getNativeInstance();
    create_info.physicalDevice = m_device->getNativeGPUHandle();
    create_info.device = m_device->getNativeHandle();
    create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

    if (const auto result = vmaCreateAllocator(&create_info, &m_handle); result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to initialize resource allocator: " +
                                                    vk::to_string(static_cast<vk::Result>(result)));
        return false;
    }

    m_encoder = m_device->createCommandEncoder("Resource_Allocator", m_device->getTransferQueue());
    m_fence = m_device->createFence("Resource_Allocator");

    return true;
}

void ResourceAllocator::destroy()
{
    const auto num_buffers = m_buffers.getSize();
    const auto num_textures = m_textures.getSize();
    const auto num_samplers = m_samplers.getSize();
    if (num_buffers > 0 || num_textures > 0 || num_samplers > 0)
    {
        core::Logger::warn(LOG_CHANNEL_VULKAN, "Some GPU resources are not destroyed. Destroying...");
    }
    if (num_buffers > 0)
    {
        for (const auto h : m_buffers.getAllHandles())
        {
            destroyBuffer(h);
        }
    }
    if (num_textures > 0)
    {
        for (const auto h : m_textures.getAllHandles())
        {
            destroyTexture(h);
        }
    }
    if (num_samplers > 0)
    {
        for (const auto h : m_samplers.getAllHandles())
        {
            destroyTextureSampler(h);
        }
    }
    m_fence.destroy();
    m_encoder.destroy();
    if (m_handle)
    {
        vmaDestroyAllocator(m_handle);
        m_handle = nullptr;
    }
}

void ResourceAllocator::beginAllocation() const
{
    m_fence.reset();
    m_encoder.begin();
}

void ResourceAllocator::endAllocation()
{
    m_device->getTransferQueue().submit(m_encoder.finish(), m_fence);
    if (!m_fence.wait(FENCE_WAIT_TIMEOUT))
    {
    }
    // Destroy all staging buffers
    for (const auto h : m_staging_buffers)
    {
        destroyBuffer(h);
    }
    m_staging_buffers.clear();
}

const Buffer *ResourceAllocator::createStagingBuffer(const std::string &name, uint64_t size, const void *data)
{
    const auto new_name = name + "_Staging";
    const BufferHandle handle =
        createBuffer(new_name, size, BufferUsageFlags::TRANSFER_SRC, nullptr, ResourceMemoryType::AUTO,
                     ResourceMemoryFlags::HOST_ACCESS_SEQUENTIAL_WRITE | ResourceMemoryFlags::MAPPED);
    const Buffer *buffer = getBuffer(handle);
    if (!buffer)
    {
        return nullptr;
    }
    if (const auto result = vmaCopyMemoryToAllocation(m_handle, data, buffer->getNativeAllocation(), 0, size);
        result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to allocate staging buffer: " + vk::to_string(static_cast<vk::Result>(result)));
        destroyBuffer(handle);
        return nullptr;
    }
    m_staging_buffers.push_back(handle);
    return buffer;
}

BufferHandle ResourceAllocator::createBuffer(const std::string &name, const uint64_t size, const BufferUsageFlags usage,
                                             const void *data, const ResourceMemoryType memory_type,
                                             const ResourceMemoryFlags memory_flags)
{
    auto create_info = VkBufferCreateInfo{};
    create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    create_info.size = size;
    create_info.usage =
        VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | static_cast<VkBufferUsageFlags>(getBufferUsageFlags(usage));

    if (data)
    {
        create_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    VmaAllocationCreateInfo alloc_create_info = {};
    alloc_create_info.usage = getMemoryUsage(memory_type);
    alloc_create_info.flags = static_cast<VmaAllocationCreateFlags>(memory_flags);

    VkBuffer vk_buffer;
    VmaAllocation alloc;
    VmaAllocationInfo info{};
    if (auto result = vmaCreateBuffer(m_handle, &create_info, &alloc_create_info, &vk_buffer, &alloc, &info);
        result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to allocate buffer in memory: " + vk::to_string(static_cast<vk::Result>(result)));
        return {};
    }

    const vk::DeviceAddress address =
        m_device->getNativeHandle().getBufferAddress(vk::BufferDeviceAddressInfo{vk_buffer});

    const BufferHandle handle = m_buffers.add(std::move(Buffer(name, size, usage, vk_buffer, alloc, info, address)));

    // set gpu buffer name for debugging
    {
        const auto vk_handle = reinterpret_cast<uint64_t>(vk_buffer);
        setDebugName(m_device->getNativeHandle(), vk::ObjectType::eBuffer, vk_handle, name);
    }

    if (data && !writeBuffer(handle, size, data))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to write buffer data");
    }

    return handle;
}

bool ResourceAllocator::writeBuffer(const BufferHandle handle, const uint64_t size, const void *data)
{
    const auto buffer = getBuffer(handle);
    if (!buffer || size == 0 || data == nullptr)
    {
        return false;
    }

    vk::MemoryPropertyFlags memory_flags = {};
    vmaGetAllocationMemoryProperties(m_handle, buffer->getNativeAllocation(),
                                     reinterpret_cast<VkMemoryPropertyFlags *>(&memory_flags));

    if (memory_flags & vk::MemoryPropertyFlagBits::eHostVisible)
    {
        // If memory is allocated in the host-visible space, copy the data directly to the given buffer.
        if (const auto result = vmaCopyMemoryToAllocation(m_handle, data, buffer->getNativeAllocation(), 0, size);
            result != VK_SUCCESS)
        {
            core::Logger::error(LOG_CHANNEL_VULKAN,
                                "Failed to copy data to buffer: " + vk::to_string(static_cast<vk::Result>(result)));
            return false;
        }

        m_encoder.addBufferBarrier(*buffer, MemoryAccessFlags::HOST_WRITE, MemoryAccessFlags::MEMORY_READ,
                                   PipelineStageFlags::HOST, PipelineStageFlags::ALL_COMMANDS);
    }
    else
    {
        // If memory is allocated in device space, create a host-visible staging buffer and then copy it to the
        // device buffer.
        const auto s_buffer = createStagingBuffer(buffer->getName(), size, data);
        if (!s_buffer)
        {
            return false;
        }

        // Copy from staging buffer to output buffer.
        m_encoder.addBufferBarrier(*s_buffer, MemoryAccessFlags::HOST_WRITE, MemoryAccessFlags::TRANSFER_READ,
                                   PipelineStageFlags::HOST, PipelineStageFlags::TRANSFER);
        m_encoder.copyBuffer(*s_buffer, *buffer, {BufferCopyRegion{0, 0, size}});
        m_encoder.addBufferBarrier(*buffer, MemoryAccessFlags::TRANSFER_WRITE, MemoryAccessFlags::MEMORY_READ,
                                   PipelineStageFlags::TRANSFER, PipelineStageFlags::ALL_COMMANDS);
    }
    return true;
}

void ResourceAllocator::destroyBuffer(const BufferHandle handle)
{
    const auto buffer = getBuffer(handle);
    if (!buffer)
    {
        return;
    }
    vmaDestroyBuffer(m_handle, buffer->getNativeHandle(), buffer->getNativeAllocation());
    m_buffers.remove(handle);
}

TextureHandle ResourceAllocator::createTexture(const std::string &name, const Size2D &size, const TextureFormat format,
                                               const TextureUsageFlags usage, const TextureLayout layout,
                                               const void *data, const ResourceMemoryType memory_type,
                                               const ResourceMemoryFlags memory_flags)
{
    const auto img_extent = getExtent3D(size);
    const auto img_format = getFormat(format);
    const auto img_usage = getImageUsageFlags(usage);

    auto create_info = VkImageCreateInfo{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.imageType = VK_IMAGE_TYPE_2D;
    create_info.format = static_cast<VkFormat>(img_format);
    create_info.extent = static_cast<VkExtent3D>(img_extent);
    create_info.mipLevels = 1;
    create_info.arrayLayers = 1;
    create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    create_info.usage = static_cast<VkImageUsageFlags>(img_usage);
    create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (data)
    {
        create_info.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VmaAllocationCreateInfo alloc_create_info = {};
    alloc_create_info.usage = getMemoryUsage(memory_type);
    alloc_create_info.flags = static_cast<VmaAllocationCreateFlagBits>(memory_flags);

    VkImage vk_image{};
    VmaAllocation alloc;
    VmaAllocationInfo info{};
    if (const auto result = vmaCreateImage(m_handle, &create_info, &alloc_create_info, &vk_image, &alloc, &info);
        result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to allocate image in memory: " + vk::to_string(static_cast<vk::Result>(result)));
        return {};
    }

    const auto img_aspect =
        isDepthTextureFormat(format) ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor;
    const auto view_create_info =
        vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{}, vk_image,
                                vk::ImageViewType::e2D,     img_format,
                                vk::ComponentMapping{},     vk::ImageSubresourceRange{img_aspect, 0, 1, 0, 1}};

    const vk::ImageView vk_view = m_device->getNativeHandle().createImageView(view_create_info);

    const TextureHandle handle = m_textures.add(
        std::move(Texture(name, size, format, usage, TextureLayout::UNDEFINED, vk_image, vk_view, alloc, info)));

    // Only when texture is created we can transition it to a layout specified in the function parameter.
    m_encoder.transitionTextureLayout(*getTexture(handle), layout);

    // set gpu image name for debugging
    {
        auto vk_handle = reinterpret_cast<uint64_t>(vk_image);
        setDebugName(m_device->getNativeHandle(), vk::ObjectType::eImage, vk_handle, name);

        const auto view_name = name + "_View";
        vk_handle = reinterpret_cast<uint64_t>(static_cast<VkImageView>(vk_view));
        setDebugName(m_device->getNativeHandle(), vk::ObjectType::eImageView, vk_handle, view_name);
    }

    if (data && !writeImage(handle, data))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to write image data");
    }

    return handle;
}

bool ResourceAllocator::writeImage(const TextureHandle handle, const void *data)
{
    const auto texture = getTexture(handle);
    if (!texture || data == nullptr)
    {
        return false;
    }

    const auto [width, height] = texture->getSize();
    const auto format = getFormat(texture->getFormat());
    const auto size = width * height * getPixelSize(format);
    const auto s_buffer = createStagingBuffer(texture->getName(), size, data);
    if (!s_buffer)
    {
        return false;
    }

    const auto init_layout = texture->getLayout();
    m_encoder.transitionTextureLayout(*texture, TextureLayout::TRANSFER_DST_OPTIMAL);
    m_encoder.copyBufferToTexture(*s_buffer, *texture);
    m_encoder.transitionTextureLayout(*texture, init_layout);

    return true;
}

void ResourceAllocator::destroyTexture(const TextureHandle handle)
{
    const auto texture = getTexture(handle);
    if (!texture)
    {
        return;
    }
    vmaDestroyImage(m_handle, texture->getNativeHandle(), texture->getNativeAllocation());
    m_device->getNativeHandle().destroyImageView(texture->getNativeViewHandle());
    m_textures.remove(handle);
}

TextureSamplerHandle ResourceAllocator::createTextureSampler(const std::string &name, const SamplerData &data)
{
    // Check for existing sampler with same state to avoid unnecessary duplicates
    TextureSamplerHandle handle = m_samplers.findIf([&data](const auto &sampler) { return sampler.getData() == data; });
    if (m_samplers.isValid(handle))
    {
        return handle;
    }

    auto create_info = vk::SamplerCreateInfo{vk::SamplerCreateFlags{},
                                             getFilter(data.mag_filter),
                                             getFilter(data.min_filter),
                                             getSamplerMipmapMode(data.mip_map_mode),
                                             getSamplerAddressMode(data.wrap_mode_u),
                                             getSamplerAddressMode(data.wrap_mode_v),
                                             vk::SamplerAddressMode::eRepeat};
    if (data.max_anisotropy > 0)
    {
        create_info.setAnisotropyEnable(true);
        create_info.setMaxAnisotropy(data.max_anisotropy);
    }
    const auto vk_sampler = m_device->getNativeHandle().createSampler(create_info);

    handle = m_samplers.add(std::move(TextureSampler(name, data, vk_sampler)));

    // set gpu sampler name for debugging
    {
        const auto vk_handle = reinterpret_cast<uint64_t>(static_cast<VkSampler>(vk_sampler));
        setDebugName(m_device->getNativeHandle(), vk::ObjectType::eSampler, vk_handle, name);
    }

    return handle;
}

void ResourceAllocator::destroyTextureSampler(const TextureSamplerHandle handle)
{
    const auto sampler = getTextureSampler(handle);
    if (!sampler)
    {
        return;
    }
    m_device->getNativeHandle().destroySampler(sampler->getNativeHandle());
    m_samplers.remove(handle);
}
} // namespace kirana::renderer