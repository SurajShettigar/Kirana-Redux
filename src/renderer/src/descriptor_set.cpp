// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_set.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
DescriptorSet::DescriptorSet(const vk::Device device, const vk::DescriptorSet handle, std::string name)
    : m_name{std::move(name)}, m_device{device}, m_handle{handle}
{
    if (!m_name.empty())
    {
        const auto id = reinterpret_cast<uint64_t>(static_cast<VkDescriptorSet>(m_handle));
        setDebugName(m_device, vk::ObjectType::eDescriptorSet, id, m_name);
    }
}

void DescriptorSet::updateBindingResources(const DescriptorLayout &layout,
                                           const std::vector<ShaderBindingResource> &resources) const
{
    std::vector<vk::WriteDescriptorSet> writes{};
    writes.reserve(resources.size());
    for (const auto &r : resources)
    {
        const auto &binding = layout.getBinding(r.index);
        if (binding.type == ShaderBindingType::STORAGE_BUFFER || binding.type == ShaderBindingType::UNIFORM_BUFFER)
        {
            vk::DescriptorBufferInfo buffer_info{r.buffer->getNativeHandle(), 0, vk::WholeSize};
            vk::WriteDescriptorSet write{m_handle, r.index, 0, getDescriptorType(binding.type), {}, {buffer_info}};
            writes.emplace_back(write);
        }
        else if (binding.type == ShaderBindingType::SAMPLED_IMAGE || binding.type == ShaderBindingType::STORAGE_IMAGE)
        {
            // TODO: Handle image descriptor binding with sampler.
            vk::DescriptorImageInfo image_info{{}, r.texture->getNativeViewHandle(),
                                               vk::ImageLayout::eGeneral};

            vk::WriteDescriptorSet write{m_handle, r.index, 0, getDescriptorType(binding.type), {image_info}};
            writes.emplace_back(write);
        }
    }
    m_device.updateDescriptorSets(writes, {});
}

}