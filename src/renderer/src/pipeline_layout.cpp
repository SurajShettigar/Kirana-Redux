// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "pipeline_layout.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool PipelineLayout::init(const vk::Device device, const std::string &name, const std::vector<DescriptorLayout> &layouts)
{
    m_device = device;

    std::vector<vk::DescriptorSetLayout> vk_layouts{};
    for (const auto &layout : layouts)
    {
        vk_layouts.emplace_back(layout.getNativeHandle());
    }

    const auto create_info = vk::PipelineLayoutCreateInfo{vk::PipelineLayoutCreateFlags{}, vk_layouts};
    m_handle = m_device.createPipelineLayout(create_info);

    if (!name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkPipelineLayout>(m_handle));
        setDebugName(m_device, vk::ObjectType::ePipelineLayout, handle, name);
    }
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