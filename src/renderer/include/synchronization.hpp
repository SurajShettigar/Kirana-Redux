// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SYNCHRONIZATION_HPP
#define KIRANA_RENDERER_SYNCHRONIZATION_HPP

#include <vulkan/vulkan.hpp>
#include "common.hpp"

namespace kirana::renderer
{
class Semaphore
{
    friend class Device;
    friend class Queue;
    friend class Swapchain;
public:
    Semaphore() = default;
    ~Semaphore() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] PipelineStageFlags getStageMask() const
    {
        return m_stage_mask;
    }

    void setStageMask(const PipelineStageFlags stage_mask)
    {
        m_stage_mask = stage_mask;
    }

    [[nodiscard]] vk::Semaphore getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};

    vk::Device m_device{nullptr};
    vk::Semaphore m_handle{nullptr};
    PipelineStageFlags m_stage_mask{PipelineStageFlags::TOP_OF_PIPE};

    bool init(vk::Device device, const std::string &name,
              PipelineStageFlags stage_mask = PipelineStageFlags::TOP_OF_PIPE);
    [[nodiscard]] vk::SemaphoreSubmitInfo getSubmitInfo() const;
};


class Fence
{
    friend class Device;
    friend class Queue;

public:
    Fence() = default;
    ~Fence() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] bool isSignaled() const;
    [[nodiscard]] bool wait(uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    void reset() const;
private:
    std::string m_name{};

    vk::Device m_device{nullptr};
    vk::Fence m_handle{nullptr};

    bool init(vk::Device device, const std::string &name);
};
}

#endif //KIRANA_RENDERER_SYNCHRONIZATION_HPP