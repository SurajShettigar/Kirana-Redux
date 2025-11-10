// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_PIPELINE_COMPUTE_HPP
#define KIRANA_RENDERER_PIPELINE_COMPUTE_HPP

#include "shader.hpp"

namespace kirana::renderer
{

class PipelineLayout;

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

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::Pipeline getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};
    vk::Device m_device{nullptr};
    vk::Pipeline m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, const PipelineLayout &layout, const Shader &shader);
};
}

#endif //KIRANA_RENDERER_PIPELINE_COMPUTE_HPP