// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "buffer.hpp"

#include "helpers_vulkan.hpp"
#include "device.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool Buffer::doCreate()
{
    m_is_dirty = false;
    const vk::Device device = m_device->getNativeHandle();

    const auto create_info = vk::BufferCreateInfo{vk::BufferCreateFlags{}, m_size, getBufferUsageFlags(m_usage)};
    if (m_allocator)
    {
        m_alloc_id = m_allocator->createBuffer(*m_encoder, create_info, m_data, &m_handle);
        if (!m_alloc_id.isValid())
        {
            return false;
        }
    }
    else
    {
        m_handle = device.createBuffer(create_info);
        if (m_data)
        {
            core::Logger::warn(LOG_CHANNEL_VULKAN, "When initializing with data, memory allocator is necessary.");
        }
    }
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkBuffer>(m_handle));
        setDebugName(device, vk::ObjectType::eBuffer, handle, m_name);
    }
    return true;
}

bool Buffer::doWrite()
{
    if (m_is_dirty)
    {
        bool is_loaded{false};
        if (m_handle && m_allocator && m_alloc_id.isValid() && m_size > 0 && m_data)
        {
            is_loaded = m_allocator->writeBuffer(*m_encoder, m_alloc_id, m_handle, m_size, m_data);
        }
        m_is_dirty = false;
        return is_loaded;
    }
    return true;
}

void Buffer::doDestroy()
{
    if (m_allocator && m_alloc_id.isValid())
    {
        m_allocator->destroyBuffer(m_alloc_id, m_handle);
    }
    else if (m_device && m_handle)
    {
        m_device->getNativeHandle().destroyBuffer(m_handle);
    }
    m_alloc_id = {};
    m_handle = nullptr;
}

bool Buffer::doLoad()
{
    if (!m_handle)
    {
        // Initializing buffer for the first time.
        return doCreate();
    }

    return doWrite();
}

void Buffer::doUnload()
{
    doDestroy();
}

uint64_t Buffer::getAddress() const
{
    return m_device->getNativeHandle().getBufferAddress(vk::BufferDeviceAddressInfo{m_handle});
}
} // namespace kirana::renderer