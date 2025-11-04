// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "synchronization.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{

bool Semaphore::init(const vk::Device device, const std::string &name, const PipelineStageFlags stage_mask)
{
    m_device = device;
    m_name = name;
    m_stage_mask = stage_mask;
    m_handle = m_device.createSemaphore(vk::SemaphoreCreateInfo{});

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkSemaphore>(m_handle));
        setDebugName(m_device, vk::ObjectType::eSemaphore, handle, m_name);
    }
    return true;
}

void Semaphore::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroySemaphore(m_handle);
        m_handle = nullptr;
    }
}

vk::SemaphoreSubmitInfo Semaphore::getSubmitInfo() const
{
    return vk::SemaphoreSubmitInfo{m_handle, {}, getPipelineStageFlags(m_stage_mask)};
}


bool Fence::init(const vk::Device device, const std::string &name)
{
    m_device = device;
    m_name = name;
    m_handle = m_device.createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkFence>(m_handle));
        setDebugName(m_device, vk::ObjectType::eFence, handle, m_name);
    }
    return true;
}

void Fence::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyFence(m_handle);
        m_handle = nullptr;
    }
}

bool Fence::isSignaled() const
{
    return m_device.getFenceStatus(m_handle) == vk::Result::eSuccess;
}

bool Fence::wait(const uint64_t timeout) const
{
    if (const auto result = m_device.waitForFences({m_handle}, true, timeout); result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to wait for fence: " + vk::to_string(result));
        return false;
    }
    return true;
}

void Fence::reset() const
{
    m_device.resetFences(m_handle);
}
}