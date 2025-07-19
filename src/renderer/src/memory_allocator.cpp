// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#define VMA_IMPLEMENTATION
#include "memory_allocator.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool MemoryAllocator::init(const vk::Instance instance, const vk::PhysicalDevice gpu, const vk::Device device)
{
    VmaAllocatorCreateInfo create_info = {};
    create_info.instance = instance;
    create_info.physicalDevice = gpu;
    create_info.device = device;
    create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    const VkResult result = vmaCreateAllocator(&create_info, &m_handle);
    return result == VK_SUCCESS;
}

void MemoryAllocator::destroy()
{
    if (!m_allocations.empty())
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Memory allocations are not destroyed.");
    }
    if (m_handle)
    {
        vmaDestroyAllocator(m_handle);
        m_handle = nullptr;
    }
}


AllocationID MemoryAllocator::createImage(const vk::ImageCreateInfo &create_info, vk::Image *out_image) const
{
    VmaAllocationCreateInfo alloc_create_info = {};
    alloc_create_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_create_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

    Allocation allocation = {};
    const auto result = vmaCreateImage(m_handle, reinterpret_cast<const VkImageCreateInfo *>(&create_info),
                                       &alloc_create_info,
                                       reinterpret_cast<VkImage *>(out_image), &allocation.handle, &allocation.info);

    if (result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to allocate image in memory: " + vk::to_string(static_cast<vk::Result>(result)));
        return {};
    }

    AllocationID id{m_allocation_count++};
    m_allocations.insert(std::make_pair(id, allocation));
    return id;
}


void MemoryAllocator::destroyImage(const AllocationID id, const vk::Image image) const
{
    if (m_allocations.find(id) == m_allocations.end())
    {
        core::Logger::warn(LOG_CHANNEL_VULKAN, "Failed to find image allocation with given id");
        return;
    }
    auto &[handle, _] = m_allocations.at(id);
    vmaDestroyImage(m_handle, image, handle);
    m_allocations.erase(id);
}

AllocationID MemoryAllocator::createBuffer(const CommandEncoder &encoder, const vk::BufferCreateInfo &create_info,
                                           const uint8_t *data_buffer, vk::Buffer *out_buffer) const
{
    auto main_create_info = create_info;
    if (create_info.size > 0 && data_buffer)
    {
        main_create_info.usage |= vk::BufferUsageFlagBits::eTransferDst;
    }

    VmaAllocationCreateInfo alloc_create_info = {};
    alloc_create_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                              | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT
                              | VMA_ALLOCATION_CREATE_MAPPED_BIT;

    Allocation allocation = {};
    auto result = vmaCreateBuffer(m_handle, reinterpret_cast<const VkBufferCreateInfo *>(&main_create_info),
                                  &alloc_create_info,
                                  reinterpret_cast<VkBuffer *>(out_buffer), &allocation.handle, &allocation.info);

    if (result != VK_SUCCESS)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to allocate buffer in memory: " + vk::to_string(static_cast<vk::Result>(result)));
        return {};
    }

    AllocationID id{m_allocation_count++};
    m_allocations.insert(std::make_pair(id, allocation));

    if (data_buffer)
    {
        vk::MemoryPropertyFlags memory_flags = {};
        vmaGetAllocationMemoryProperties(m_handle, allocation.handle,
                                         reinterpret_cast<VkMemoryPropertyFlags *>(&memory_flags));


        if (memory_flags & vk::MemoryPropertyFlagBits::eHostVisible)
        {
            // If memory is allocated in the host-visible space, copy the data directly to the given buffer.
            result = vmaCopyMemoryToAllocation(m_handle, data_buffer, allocation.handle, 0, main_create_info.size);
            if (result != VK_SUCCESS)
            {
                core::Logger::error(LOG_CHANNEL_VULKAN,
                                    "Failed to copy data to buffer: " + vk::to_string(static_cast<vk::Result>(result)));
            }
            // In case of single queue, add a barrier to ensure the transfer is completed before any stage.
            const auto barrier = vk::BufferMemoryBarrier2{vk::PipelineStageFlagBits2::eHost,
                                                          vk::AccessFlagBits2::eHostWrite,
                                                          vk::PipelineStageFlagBits2::eAllCommands,
                                                          vk::AccessFlagBits2::eMemoryRead, vk::QueueFamilyIgnored,
                                                          vk::QueueFamilyIgnored, *out_buffer, 0,
                                                          main_create_info.size};
            encoder.getNativeHandle().pipelineBarrier2(vk::DependencyInfo{vk::DependencyFlags{}, {}, {barrier}});
        }
        else
        {
            // If memory is allocated in device space, create a host-visible staging buffer and then copy it to the
            // device buffer.
            vk::Buffer s_buffer = {};
            VmaAllocationCreateInfo s_alloc_create_info = {};
            s_alloc_create_info.usage = VMA_MEMORY_USAGE_AUTO;
            s_alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                                        | VMA_ALLOCATION_CREATE_MAPPED_BIT;
            auto s_create_info = create_info;
            s_create_info.usage |= vk::BufferUsageFlagBits::eTransferSrc;
            Allocation s_alloc = {};
            const auto s_result = vmaCreateBuffer(
                m_handle, reinterpret_cast<const VkBufferCreateInfo *>(&s_create_info),
                &s_alloc_create_info,
                reinterpret_cast<VkBuffer *>(&s_buffer), &s_alloc.handle, &s_alloc.info);
            if (s_result != VK_SUCCESS)
            {
                core::Logger::error(LOG_CHANNEL_VULKAN,
                                    "Failed to allocate staging buffer for copy: " + vk::to_string(
                                        static_cast<vk::Result>(s_result)));
            }
            else
            {
                const auto c_result = vmaCopyMemoryToAllocation(m_handle, data_buffer, s_alloc.handle, 0,
                                                                s_create_info.size);
                if (c_result != VK_SUCCESS)
                {
                    core::Logger::error(LOG_CHANNEL_VULKAN,
                                        "Failed to copy data to buffer: " + vk::to_string(
                                            static_cast<vk::Result>(c_result)));
                }
                else
                {
                    // Copy from staging buffer to output buffer.
                    auto barrier = vk::BufferMemoryBarrier2{vk::PipelineStageFlagBits2::eHost,
                                                            vk::AccessFlagBits2::eHostWrite,
                                                            vk::PipelineStageFlagBits2::eTransfer,
                                                            vk::AccessFlagBits2::eTransferRead,
                                                            vk::QueueFamilyIgnored,
                                                            vk::QueueFamilyIgnored, s_buffer, 0, s_create_info.size};
                    encoder.getNativeHandle().
                            pipelineBarrier2(vk::DependencyInfo{vk::DependencyFlags{}, {}, {barrier}});

                    const std::vector copy_regions = {vk::BufferCopy2{0, 0, s_create_info.size}};
                    encoder.getNativeHandle().copyBuffer2(vk::CopyBufferInfo2{s_buffer, *out_buffer, copy_regions});

                    barrier = vk::BufferMemoryBarrier2{vk::PipelineStageFlagBits2::eTransfer,
                                                       vk::AccessFlagBits2::eTransferWrite,
                                                       vk::PipelineStageFlagBits2::eAllCommands,
                                                       vk::AccessFlagBits2::eMemoryRead, vk::QueueFamilyIgnored,
                                                       vk::QueueFamilyIgnored, *out_buffer, 0, s_create_info.size};
                    encoder.getNativeHandle().
                            pipelineBarrier2(vk::DependencyInfo{vk::DependencyFlags{}, {}, {barrier}});
                }

                AllocationID s_id{m_allocation_count++};
                m_allocations.insert(std::make_pair(s_id, s_alloc));
                m_staging_buffers.insert(std::make_pair(s_id, s_buffer));
            }
        }
    }

    return id;
}

void MemoryAllocator::destroyBuffer(const AllocationID id, const vk::Buffer buffer) const
{
    if (m_allocations.find(id) == m_allocations.end())
    {
        core::Logger::warn(LOG_CHANNEL_VULKAN, "Failed to find buffer allocation with given id");
        return;
    }
    auto &[handle, _] = m_allocations.at(id);
    vmaDestroyBuffer(m_handle, buffer, handle);
    m_allocations.erase(id);
}

bool MemoryAllocator::tryReleaseTemporaries(const Fence &fence) const
{
    if (fence.isValid() && fence.isSignaled())
    {
        for (const auto &[id, buffer] : m_staging_buffers)
        {
            vmaDestroyBuffer(m_handle, buffer, m_allocations.at(id).handle);
        }
        m_staging_buffers.clear();
        return true;
    }
    return false;
}
}