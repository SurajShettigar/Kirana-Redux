// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_QUEUE_HPP
#define KIRANA_RENDERER_QUEUE_HPP

#include "command_encoder.hpp"
#include "swapchain.hpp"
#include "synchronization.hpp"

namespace kirana::renderer
{
class Queue
{
    friend class Device;

public:
    Queue() = default;

    Queue(const std::string &name, const uint32_t index, const uint32_t family_index, const QueueFamilyFlags type)
        : m_name{name}, m_index{index}, m_family_index{family_index}, m_type{type}
    {
    }

    ~Queue() = default;

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr
               && m_index < std::numeric_limits<uint32_t>::max()
               && m_family_index < std::numeric_limits<uint32_t>::max()
               && m_type != QueueFamilyFlags::NONE;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] uint32_t getIndex() const
    {
        return m_index;
    }

    [[nodiscard]] uint32_t getFamilyIndex() const
    {
        return m_family_index;
    }

    [[nodiscard]] QueueFamilyFlags getType() const
    {
        return m_type;
    }

    void addWaitSemaphore(const Semaphore &semaphore) const
    {
        m_wait_semaphores.emplace_back(semaphore.getSubmitInfo());
    }

    void addSignalSemaphore(const Semaphore &semaphore) const
    {
        m_signal_semaphores.emplace_back(semaphore.getSubmitInfo());
    }

    void submit(const CommandSubmitInfo &cmd_submit_info, const Fence &fence) const;
    bool present(Swapchain &swapchain) const;

private:
    std::string m_name{};
    uint32_t m_index = std::numeric_limits<uint32_t>::max();
    uint32_t m_family_index = std::numeric_limits<uint32_t>::max();
    QueueFamilyFlags m_type = QueueFamilyFlags::NONE;

    vk::Device m_device{nullptr};
    vk::Queue m_handle{nullptr};
    mutable std::vector<vk::SemaphoreSubmitInfo> m_wait_semaphores{};
    mutable std::vector<vk::SemaphoreSubmitInfo> m_signal_semaphores{};

    Queue(vk::Device device, const std::string &name, uint32_t index, uint32_t family_index, QueueFamilyFlags type);
};
}

#endif //KIRANA_RENDERER_QUEUE_HPP