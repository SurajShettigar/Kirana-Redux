// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_layout.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
constexpr vk::DescriptorBindingFlags BINDLESS_FLAGS = vk::DescriptorBindingFlagBits::ePartiallyBound;

bool DescriptorLayout::init(const vk::Device device, const std::string &name, const ShaderStageFlags shader_stages,
                            const std::vector<ShaderBinding> &bindings)
{
    m_device = device;
    m_name = name;
    m_shader_stages = shader_stages;

    for (const auto b : bindings)
    {
        addBinding(b);
    }

    if (m_bindings.empty())
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Descriptor layout has no bindings. Either add them before calling init or pass"
                            "them through the init function parameter.");
        return false;
    }
    std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
    std::vector<vk::DescriptorBindingFlags> vk_binding_flags{};
    vk_bindings.reserve(m_bindings.size());
    vk_binding_flags.reserve(m_bindings.size());
    for (const auto &[index, b] : m_bindings)
    {
        const auto stages = b.shader_stages | m_shader_stages;
        vk_bindings.emplace_back(index, getDescriptorType(b.type), b.count, getShaderStageFlags(stages));
        vk_binding_flags.emplace_back(b.bindless ? BINDLESS_FLAGS : vk::DescriptorBindingFlags{});
    }

    const auto bindless_flags_create_info = vk::DescriptorSetLayoutBindingFlagsCreateInfo{vk_binding_flags};
    const auto create_info = vk::DescriptorSetLayoutCreateInfo{vk::DescriptorSetLayoutCreateFlags{}, vk_bindings,
                                                               &bindless_flags_create_info};
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
        clearBindings();
        m_handle = nullptr;
    }
}

} // namespace kirana::renderer