// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "queue.hpp"
#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
Queue::Queue(const vk::Device device, const uint32_t index, const uint32_t family_index, const QueueFamilyFlags type)
    : m_device{device}, m_index{index}, m_family_index{family_index}, m_type{type},
      m_handle{device.getQueue(family_index, index)}
{
}


void Queue::submit(const CommandSubmitInfo &cmd_submit_info, const Fence &fence)
{
    const auto submit_info = vk::SubmitInfo2{vk::SubmitFlags{}, m_wait_semaphores, {cmd_submit_info.info},
                                             m_signal_semaphores};
    m_handle.submit2({submit_info}, fence.m_handle);
    m_wait_semaphores.clear();
    m_signal_semaphores.clear();
}
}

bool kirana::renderer::Queue::present(const SwapchainPresentInfo &swapchain_info)
{
    std::vector<vk::Semaphore> wait_semaphores;
    wait_semaphores.reserve(m_wait_semaphores.size());
    for (const auto &semaphore : m_wait_semaphores)
    {
        wait_semaphores.emplace_back(semaphore.semaphore);
    }
    bool status = true;

    // We use C-style present api function because the hpp one would throw an exception and crash when the swapchain
    // present fails.
    const auto present_info = vk::PresentInfoKHR
        {wait_semaphores, {swapchain_info.handle}, {swapchain_info.image_index}};
    if (const VkResult result = vkQueuePresentKHR(m_handle, reinterpret_cast<const VkPresentInfoKHR *>(&present_info));
        static_cast<vk::Result>(result) != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to present swapchain image: " + vk::to_string(static_cast<vk::Result>(result)));
        status = false;
    }
    m_wait_semaphores.clear();
    m_signal_semaphores.clear();
    return status;
}