// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "queue.hpp"
#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
Queue::Queue(const vk::Device device, const std::string &name, const uint32_t index, const uint32_t family_index,
             const QueueFamilyFlags type)
    : m_name{name}, m_index{index}, m_family_index{family_index}, m_type{type}, m_device{device},
      m_handle{m_device.getQueue(family_index, index)}
{
    if (!name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkQueue>(m_handle));
        setDebugName(m_device, vk::ObjectType::eQueue, handle, name);
    }
}

void Queue::submit(const CommandSubmitInfo &cmd_submit_info, const Fence &fence) const
{
    const auto submit_info = vk::SubmitInfo2{vk::SubmitFlags{}, m_wait_semaphores, {cmd_submit_info.info},
                                             m_signal_semaphores};
    m_handle.submit2({submit_info}, fence.m_handle);
    m_wait_semaphores.clear();
    m_signal_semaphores.clear();
}
}

bool kirana::renderer::Queue::present(Swapchain &swapchain) const
{
    std::vector<vk::Semaphore> wait_semaphores;
    wait_semaphores.reserve(m_wait_semaphores.size());
    for (const auto &semaphore : m_wait_semaphores)
    {
        wait_semaphores.emplace_back(semaphore.semaphore);
    }
    bool status = true;

    const auto [swapchain_handle, swapchain_image_index] = swapchain.present();
    const auto present_info = vk::PresentInfoKHR
        {wait_semaphores, {swapchain_handle}, {swapchain_image_index}};

    // We use C-style present api function because the hpp one would throw an exception and crash when the swapchain
    // present fails.
    const VkResult vk_result = vkQueuePresentKHR(m_handle, reinterpret_cast<const VkPresentInfoKHR *>(&present_info));
    if (const auto result = static_cast<vk::Result>(vk_result); result == vk::Result::eErrorOutOfDateKHR)
    {
        status = swapchain.reinitialize();
    }
    else if (result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to present swapchain image: " + vk::to_string(result));
        status = false;
    }

    m_wait_semaphores.clear();
    m_signal_semaphores.clear();
    return status;
}