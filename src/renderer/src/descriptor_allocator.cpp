// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_allocator.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool DescriptorAllocator::init(const vk::Device device, const std::string &name,
                               const std::vector<ShaderBindingTypeRatios> &binding_type_ratios, const uint32_t max_sets)
{
    m_device = device;
    m_name = name;

    if (binding_type_ratios.empty())
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "\"binding_type_ratios\" cannot be empty. The type of bindings that this "
                            "allocator can allocate needs to be defined.");
    }
    std::vector<vk::DescriptorPoolSize> pool_sizes{};
    pool_sizes.reserve(binding_type_ratios.size());
    const uint32_t default_binding_count = max_sets / binding_type_ratios.size();
    for (const auto &[type, ratio] : binding_type_ratios)
    {
        const float r = std::fmin(std::fmax(ratio, 0.0f), 1.0f);
        const uint32_t count =
            r > 0.0f ? static_cast<uint32_t>(r * static_cast<float>(max_sets)) : default_binding_count;
        pool_sizes.emplace_back(getDescriptorType(type), count);
    }

    const auto create_info =
        vk::DescriptorPoolCreateInfo{vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind, max_sets, pool_sizes};
    m_handle = m_device.createDescriptorPool(create_info);
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkDescriptorPool>(m_handle));
        setDebugName(m_device, vk::ObjectType::eDescriptorPool, handle, m_name);
    }
    return true;
}

void DescriptorAllocator::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyDescriptorPool(m_handle);
        m_handle = nullptr;
    }
}

DescriptorSet DescriptorAllocator::allocate(const std::string &name, const DescriptorLayout &layout,
                                            const std::vector<ShaderBindingResource> &binding_resources) const
{
    const std::vector layout_handles = {layout.getNativeHandle()};
    auto allocate_info = vk::DescriptorSetAllocateInfo{m_handle, layout_handles};

    std::vector descriptor_counts{layout.getTotalBindingCount()};
    const vk::DescriptorSetVariableDescriptorCountAllocateInfo variable_info{descriptor_counts};

    allocate_info.pNext = &variable_info;

    const auto vk_set = m_device.allocateDescriptorSets(allocate_info)[0];
    const auto set = DescriptorSet{m_device, vk_set, name};
    if (!binding_resources.empty())
    {
        set.updateBindingResources(layout, binding_resources);
    }
    return set;
}

void DescriptorAllocator::reset() const
{
    m_device.resetDescriptorPool(m_handle, vk::DescriptorPoolResetFlags{});
}

} // namespace kirana::renderer