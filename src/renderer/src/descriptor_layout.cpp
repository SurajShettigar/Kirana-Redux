// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_layout.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool DescriptorLayout::init(const vk::Device device, const std::string &name, const ShaderStageFlags shader_stages,
                            const std::vector<ShaderBinding> &bindings)
{
    m_device = device;
    m_name = name;

    if (!bindings.empty())
    {
        m_bindings.reserve(m_bindings.size() + bindings.size());
        m_bindings.insert(m_bindings.end(), bindings.begin(), bindings.end());
    }

    if (m_bindings.empty())
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Descriptor layout has no bindings. Either add them before calling init or pass"
                            "them through the init function parameter.");
        return false;
    }
    std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
    vk_bindings.reserve(m_bindings.size());
    for (const auto &b : m_bindings)
    {
        const auto stages = b.shader_stages | shader_stages;
        vk_bindings.emplace_back(b.index, getDescriptorType(b.type), 1, getShaderStageFlags(stages));
    }

    const auto create_info = vk::DescriptorSetLayoutCreateInfo{vk::DescriptorSetLayoutCreateFlags{}, vk_bindings};
    m_handle = m_device.createDescriptorSetLayout(create_info);
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkDescriptorSetLayout>(m_handle));
        setDebugName(m_device, vk::ObjectType::eDescriptorSetLayout, handle, m_name);
    }
    return true;
}

void DescriptorLayout::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyDescriptorSetLayout(m_handle);
        m_bindings.clear();
        m_handle = nullptr;
    }
}

}