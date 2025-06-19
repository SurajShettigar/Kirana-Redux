// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DEVICE_HPP
#define KIRANA_RENDERER_DEVICE_HPP

#include <vulkan/vulkan.hpp>

#include "common.hpp"
#include "no_copy.hpp"

namespace kirana::renderer
{
class Device : core::NoCopy
{
    friend VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
        vk::DebugUtilsMessageTypeFlagsEXT message_types,
        const vk::DebugUtilsMessengerCallbackDataEXT *p_callback_data,
        void *p_user_data);

public:
    Device() = default;
    ~Device();

    bool init(const DeviceInitializationData &init_data);

    bool isInitialized() const
    {
        return m_instance != nullptr && m_device != nullptr && !m_queues.empty();
    }

    bool supportsAsyncCompute() const
    {
        return m_queue_index_compute > 0;
    }

    bool supportsAsyncTransfer() const
    {
        return m_queue_index_transfer > 0;
    }

    void destroy();

private:
    vk::Instance m_instance = nullptr;
    vk::DebugUtilsMessengerEXT m_debug_messenger = nullptr;
    vk::SurfaceKHR m_surface = nullptr;

    vk::PhysicalDevice m_gpu = nullptr;
    vk::Device m_device = nullptr;

    std::vector<Queue> m_queues = {};
    uint32_t m_queue_index_compute = 0;
    uint32_t m_queue_index_transfer = 0;
};
}
#endif // KIRANA_RENDERER_DEVICE_HPP