// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "buffer.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool Buffer::init(const vk::Device device, const MemoryAllocator *allocator, const CommandEncoder &encoder,
                  const std::string &name, const uint64_t size, const void *data, const BufferUsageFlags usage)
{
    m_device = device;
    m_allocator = allocator;

    m_name = name;
    m_size = size;
    m_usage = usage;

    const auto create_info = vk::BufferCreateInfo{vk::BufferCreateFlags{}, m_size, getBufferUsageFlags(m_usage)};
    if (m_allocator)
    {
        m_alloc_id = m_allocator->createBuffer(encoder, create_info, data, &m_handle);
        if (!m_alloc_id.isValid())
        {
            return false;
        }
    }
    else
    {
        m_handle = m_device.createBuffer(create_info);
        if (data)
        {
            core::Logger::warn(LOG_CHANNEL_VULKAN, "When initializing with data, memory allocator is necessary.");
        }
    }
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkBuffer>(m_handle));
        setDebugName(m_device, vk::ObjectType::eBuffer, handle, m_name);
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

uint64_t Buffer::getAddress() const
{
    return m_device.getBufferAddress(vk::BufferDeviceAddressInfo{m_handle});
}

bool Buffer::update(const CommandEncoder &encoder, const uint64_t size, const void *data)
{
    if (m_allocator && m_alloc_id.isValid() && m_allocator->writeBuffer(encoder, m_alloc_id, m_handle, size, data))
    {
        m_size = size;
        return true;
    }
    return false;
}

}