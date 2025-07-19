// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_PIPELINE_LAYOUT_HPP
#define KIRANA_RENDERER_PIPELINE_LAYOUT_HPP

#include "descriptor_layout.hpp"

namespace kirana::renderer
{
class PipelineLayout
{
    friend class Device;

public:
    PipelineLayout() = default;
    ~PipelineLayout() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::PipelineLayout getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};

    vk::Device m_device{nullptr};
    vk::PipelineLayout m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, const std::vector<DescriptorLayout> &layouts);
};
}

#endif //KIRANA_RENDERER_PIPELINE_LAYOUT_HPP