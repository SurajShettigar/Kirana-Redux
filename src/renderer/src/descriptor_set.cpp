// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "descriptor_set.hpp"

#include "helpers_vulkan.hpp"
#include "device.hpp"

namespace kirana::renderer
{
DescriptorSet::DescriptorSet(const Device *device, std::string name, const vk::DescriptorSet handle)
    : m_device{device}, m_name{std::move(name)}, m_handle{handle}
{
    if (!m_name.empty())
    {
        const auto id = reinterpret_cast<uint64_t>(static_cast<VkDescriptorSet>(m_handle));
        setDebugName(m_device->getNativeHandle(), vk::ObjectType::eDescriptorSet, id, m_name);
    }
}

void DescriptorSet::updateBindingResources(const DescriptorLayout &layout,
                                           const std::vector<ShaderBindingResource> &resources) const
{
    // We store the buffer / texture infos outside the scope of for loop to avoid invalid buffer handles.
    // (As vk::WriteDescriptorSet takes a pointer to the vector of infos, they become invalid once outside the for
    // loop).
    std::vector<std::vector<vk::DescriptorImageInfo>> image_infos{};
    std::vector<std::vector<vk::DescriptorBufferInfo>> buffer_infos{};
    std::vector<vk::WriteDescriptorSet> writes{};
    writes.reserve(resources.size());
    for (const auto &r : resources)
    {
        const auto &binding = layout.getBinding(r.index);
        if (binding.type == ShaderBindingType::STORAGE_BUFFER || binding.type == ShaderBindingType::UNIFORM_BUFFER)
        {
            std::vector<vk::DescriptorBufferInfo> buffer_info{};
            for (const auto h : r.buffers())
            {
                const auto buf = m_device->getBuffer(h);
                buffer_info.emplace_back(buf->getNativeHandle(), 0, vk::WholeSize);
            }
            buffer_infos.emplace_back(std::move(buffer_info));
            vk::WriteDescriptorSet write{m_handle,           r.index, 0, getDescriptorType(binding.type), {},
                                         buffer_infos.back()};
            writes.emplace_back(write);
        }
        else if (binding.type == ShaderBindingType::COMBINED_IMAGE_SAMPLER ||
                 binding.type == ShaderBindingType::SAMPLED_IMAGE || binding.type == ShaderBindingType::STORAGE_IMAGE)
        {
            std::vector<vk::DescriptorImageInfo> image_info{};
            const auto &textures = r.textures();
            const auto &samplers = r.samplers;
            for (size_t i = 0; i < textures.size(); ++i)
            {
                auto sampler = vk::Sampler{};
                if (binding.type == ShaderBindingType::COMBINED_IMAGE_SAMPLER && i < samplers.size())
                {
                    sampler = m_device->getTextureSampler(samplers[i])->getNativeHandle();
                }
                const auto tex = m_device->getTexture(textures[i]);
                image_info.emplace_back(sampler, tex->getNativeViewHandle(), getImageLayout(tex->getLayout()));
            }
            image_infos.emplace_back(std::move(image_info));
            vk::WriteDescriptorSet write{m_handle, r.index, 0, getDescriptorType(binding.type), image_infos.back()};
            writes.emplace_back(write);
        }
        else if (binding.type == ShaderBindingType::SAMPLER)
        {
            std::vector<vk::DescriptorImageInfo> image_info{};
            for (const auto &h : r.samplers)
            {
                const auto sampler = m_device->getTextureSampler(h);
                image_info.emplace_back(sampler->getNativeHandle());
            }
            image_infos.emplace_back(std::move(image_info));
            vk::WriteDescriptorSet write{m_handle, r.index, 0, getDescriptorType(binding.type), image_infos.back()};
            writes.emplace_back(write);
        }
    }
    m_device->getNativeHandle().updateDescriptorSets(writes, {});
}

} // namespace kirana::renderer