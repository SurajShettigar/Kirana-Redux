// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "queue.hpp"

namespace kirana::renderer
{
Queue::Queue(const vk::Device device, const uint32_t index, const uint32_t family_index, const QueueFamilyFlags type)
    : m_device{device}, m_index{index}, m_family_index{family_index}, m_type{type},
      m_handle{device.getQueue(family_index, index)}
{
}


void Queue::submit(const CommandSubmitInfo &submit_info) const
{

}

void Queue::present(const SwapchainTexture &swapchain_texture) const
{

}

}