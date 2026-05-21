// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DESCRIPTOR_ALLOCATOR_HPP
#define KIRANA_RENDERER_DESCRIPTOR_ALLOCATOR_HPP

#include "descriptor_layout.hpp"
#include "descriptor_set.hpp"

namespace kirana::renderer
{
/**
 * The fraction of total descriptor counts which can be allocated of the given binding type. This is used to initialize
 * descriptor allocator.
 */
struct ShaderBindingTypeRatios
{
    ShaderBindingType type = ShaderBindingType::UNKNOWN;
    float ratio = 0.0f;
};

class Device;
class DescriptorAllocator
{
    friend class Device;

  public:
    DescriptorAllocator() = default;
    ~DescriptorAllocator() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] DescriptorSet allocate(const std::string &name, const DescriptorLayout &layout,
                                         const std::vector<ShaderBindingResource> &binding_resources = {}) const;
    void reset() const;

  private:
    const Device *m_device{nullptr};

    std::string m_name{};
    vk::DescriptorPool m_handle{nullptr};

    bool init(const Device *device, const std::string &name,
              const std::vector<ShaderBindingTypeRatios> &binding_type_ratios, uint32_t max_sets = 16384);
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_DESCRIPTOR_ALLOCATOR_HPP