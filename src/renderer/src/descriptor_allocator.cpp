// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_allocator.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>
#include <winsock2.h>


namespace kirana::renderer
{
bool DescriptorAllocator::init(vk::Device device, const std::vector<ShaderBindingTypeRatios> &binding_type_ratios,
                               uint32_t max_sets)
{
    m_device = device;

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
        const uint32_t count = r > 0.0f
                                   ? static_cast<uint32_t>(r * static_cast<float>(max_sets))
                                   : default_binding_count;
        pool_sizes.emplace_back(getDescriptorType(type), count);
    }

    const auto create_info = vk::DescriptorPoolCreateInfo{vk::DescriptorPoolCreateFlags{}, max_sets, pool_sizes};
    m_handle = m_device.createDescriptorPool(create_info);
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

DescriptorSet DescriptorAllocator::allocate(const DescriptorLayout &layout,
                                            const std::vector<ShaderBindingResource> &resources) const
{
    const std::vector layout_handles = {layout.getNativeHandle()};
    const auto allocate_info = vk::DescriptorSetAllocateInfo{m_handle, layout_handles};
    const auto vk_set = m_device.allocateDescriptorSets(allocate_info)[0];
    const auto set = DescriptorSet{m_device, vk_set};
    if (!resources.empty())
    {
        set.updateBindingResources(layout, resources);
    }
    return set;
}

void DescriptorAllocator::reset() const
{
    m_device.resetDescriptorPool(m_handle, vk::DescriptorPoolResetFlags{});
}

}