// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_set.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
void DescriptorSet::updateBindingResources(const DescriptorLayout &layout,
                                           const std::vector<ShaderBindingResource> &resources) const
{
    std::vector<vk::WriteDescriptorSet> writes{};
    writes.reserve(resources.size());
    for (const auto &r : resources)
    {
        const auto &binding = layout.getBinding(r.index);
        if (binding.type == ShaderBindingType::SAMPLED_IMAGE || binding.type == ShaderBindingType::STORAGE_IMAGE)
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