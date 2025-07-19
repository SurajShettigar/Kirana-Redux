// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DESCRIPTOR_SET_HPP
#define KIRANA_RENDERER_DESCRIPTOR_SET_HPP

#include <descriptor_layout.hpp>
#include "texture.hpp"

namespace kirana::renderer
{
struct ShaderBindingResource
{
    uint32_t index = 0;
    union
    {
        const Texture *texture = nullptr;
    };
};

class DescriptorSet
{
    friend class DescriptorAllocator;

public:
    DescriptorSet() = default;
    ~DescriptorSet() = default;

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::DescriptorSet getNativeHandle() const
    {
        return m_handle;
    }

    void updateBindingResources(const DescriptorLayout &layout,
                                const std::vector<ShaderBindingResource> &resources) const;

private:
    std::string m_name{};
    vk::Device m_device{nullptr};
    vk::DescriptorSet m_handle{nullptr};

    DescriptorSet(vk::Device device, vk::DescriptorSet handle, std::string name);
};
}

#endif //KIRANA_RENDERER_DESCRIPTOR_SET_HPP