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
        m_binding_count += binding.count;
        m_bindings[binding.index] = binding;
    }

    [[nodiscard]] const ShaderBinding &getBinding(const uint32_t index) const
    {
        return m_bindings.at(index);
    }

    void clearBindings()
    {
        m_binding_count = 0;
        m_bindings.clear();
    }

    [[nodiscard]] uint32_t getTotalBindingCount() const
    {
        return m_binding_count;
    }

  private:
    std::string m_name{};
    ShaderStageFlags m_shader_stages{};
    std::unordered_map<uint32_t, ShaderBinding> m_bindings{};
    uint32_t m_binding_count{};

    vk::Device m_device{nullptr};
    vk::DescriptorSetLayout m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, ShaderStageFlags shader_stages,
              const std::vector<ShaderBinding> &bindings = {});
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_DESCRIPTOR_LAYOUT_HPP