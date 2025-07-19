// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DESCRIPTOR_LAYOUT_HPP
#define KIRANA_RENDERER_DESCRIPTOR_LAYOUT_HPP

#include <vulkan/vulkan.hpp>

#include "common.hpp"

namespace kirana::renderer
{
class DescriptorLayout
{
    friend class Device;

public:
    DescriptorLayout() = default;
    ~DescriptorLayout() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::DescriptorSetLayout getNativeHandle() const
    {
        return m_handle;
    }

    void addBinding(const ShaderBinding &binding)
    {
        m_bindings.push_back(binding);
    }

    [[nodiscard]] const ShaderBinding &getBinding(const size_t index) const
    {
        return m_bindings[index];
    }

    void clearBindings()
    {
        m_bindings.clear();
    }

private:
    std::string m_name{};
    std::vector<ShaderBinding> m_bindings{};

    vk::Device m_device{nullptr};
    vk::DescriptorSetLayout m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, ShaderStageFlags shader_stages,
              const std::vector<ShaderBinding> &bindings = {});
};
}

#endif //KIRANA_RENDERER_DESCRIPTOR_LAYOUT_HPP