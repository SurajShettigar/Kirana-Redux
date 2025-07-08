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
    const auto &[handle, _] = m_allocations.at(id);
    vmaDestroyImage(m_handle, image, handle);
    m_allocations.erase(id);
}
}