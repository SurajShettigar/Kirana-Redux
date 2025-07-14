// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "buffer.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool Buffer::init(const vk::Device device, const uint64_t size, const BufferUsageFlags usage,
                  const MemoryAllocator *allocator)
{
    m_device = device;
    m_allocator = allocator;

    const auto create_info = vk::BufferCreateInfo{vk::BufferCreateFlags{}, size, getBufferUsageFlags(usage)};
    if (m_allocator)
    {
        m_alloc_id = m_allocator->createBuffer(create_info, &m_handle);
        if (!m_alloc_id.isValid())
        {
            return false;
        }
    }
    else
    {
        m_handle = m_device.createBuffer(create_info);
    }
    return true;
}

void Buffer::destroy()
{
    if (m_allocator && m_alloc_id.isValid())
    {
        m_allocator->destroyBuffer(m_alloc_id, m_handle);
    }
    else if (m_device && m_handle)
    {
        m_device.destroyBuffer(m_handle);
        m_handle = nullptr;
    }
}


}