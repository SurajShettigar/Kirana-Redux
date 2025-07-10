// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "pipeline_layout.hpp"

namespace kirana::renderer
{
bool PipelineLayout::init(const vk::Device device, const std::vector<DescriptorLayout> &layouts)
{
    m_device = device;

    std::vector<vk::DescriptorSetLayout> vk_layouts{};
    for (const auto &layout : layouts)
    {
        vk_layouts.emplace_back(layout.getNativeHandle());
    }

    const auto create_info = vk::PipelineLayoutCreateInfo{vk::PipelineLayoutCreateFlags{}, vk_layouts};
    m_handle = m_device.createPipelineLayout(create_info);
    return true;
}

void PipelineLayout::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyPipelineLayout(m_handle);
        m_handle = nullptr;
    }
}


}