// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_PIPELINE_COMPUTE_HPP
#define KIRANA_RENDERER_PIPELINE_COMPUTE_HPP

#include "pipeline_layout.hpp"
#include "shader.hpp"

namespace kirana::renderer
{
class PipelineCompute
{
    friend class Device;

public:
    PipelineCompute() = default;
    ~PipelineCompute() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] vk::Pipeline getNativeHandle() const
    {
        return m_handle;
    }

private:
    vk::Device m_device{nullptr};
    vk::Pipeline m_handle{nullptr};

    bool init(vk::Device device, const PipelineLayout &layout, const Shader &shader);
};
}

#endif //KIRANA_RENDERER_PIPELINE_COMPUTE_HPP
