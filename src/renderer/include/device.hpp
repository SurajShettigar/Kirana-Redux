// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DEVICE_HPP
#define KIRANA_RENDERER_DEVICE_HPP

#include <vulkan/vulkan.hpp>
#include "common.hpp"

namespace kirana::renderer
{
class Device
{
    friend VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
        vk::DebugUtilsMessageTypeFlagsEXT message_types,
        const vk::DebugUtilsMessengerCallbackDataEXT *p_callback_data,
        void *p_user_data);

    friend class CommandEncoder;
    friend class Swapchain;
    friend class Texture;

public:
    Device() = default;
    ~Device() = default;

    bool init(const DeviceInitializationData &init_data);
    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_instance != nullptr && m_device != nullptr && !m_queues.empty();
    }

    [[nodiscard]] bool supportsAsyncCompute() const
    {
        return m_queue_index_compute > 0;
    }

    [[nodiscard]] bool supportsAsyncTransfer() const
    {
        return m_queue_index_transfer > 0;
    }

    [[nodiscard]] const Queue &getGraphicsQueueInfo() const
    {
        return m_queue_infos[0];
    }

    [[nodiscard]] const Queue &getPresentQueueInfo() const
    {
        return m_queue_infos[0];
    }

    [[nodiscard]] const Queue &getComputeQueueInfo() const
    {
        return m_queue_infos[m_queue_index_compute];
    }

    [[nodiscard]] const Queue &getTransferQueueInfo() const
    {
        return m_queue_infos[m_queue_index_transfer];
    }

private:
    vk::Instance m_instance{nullptr};
    vk::DebugUtilsMessengerEXT m_debug_messenger{nullptr};

    vk::SurfaceKHR m_surface{nullptr};

    vk::PhysicalDevice m_gpu{nullptr};
    vk::Device m_device{nullptr};

    std::vector<Queue> m_queue_infos{};
    std::vector<vk::Queue> m_queues{};
    uint32_t m_queue_index_compute{0};
    uint32_t m_queue_index_transfer{0};

    [[nodiscard]] vk::Queue getGraphicsQueue() const
    {
        return m_queues[0];
    }

    [[nodiscard]] vk::Queue getPresentQueue() const
    {
        return m_queues[0];
    }
    [[nodiscard]] vk::Queue getComputeQueue() const
    {
        return m_queues[m_queue_index_compute];
    }

    [[nodiscard]] vk::Queue getTransferQueue() const
    {
        return m_queues[m_queue_index_transfer];
    }
};
}
#endif // KIRANA_RENDERER_DEVICE_HPP