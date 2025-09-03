// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "device.hpp"

#include "helpers_vulkan.hpp"
#include "queue.hpp"

#include <map>
#include <logger.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#if PLATFORM == WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
const auto PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#elif PLATFORM == LINUX
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan_xlib.h>
const auto PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
#elif PLATFORM == MAC
#define VK_USE_PLATFORM_METAL_EXT
#include <vulkan/vulkan_metal.h>
const auto PLATFORM_SURFACE_EXTENSION_NAME = VK_EXT_METAL_SURFACE_EXTENSION_NAME;
#endif


namespace kirana::renderer
{
struct QueueInfo
{
    std::string name;
    uint32_t index;
    uint32_t family_index;
    QueueFamilyFlags type;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL
validationCallback(const vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
                   const vk::DebugUtilsMessageTypeFlagsEXT message_types,
                   const vk::DebugUtilsMessengerCallbackDataEXT *p_callback_data,
                   void *p_user_data)
{
    // const Device* device = reinterpret_cast<Device*>(p_user_data);
    const auto &message = [&]() -> std::string {
        return vk::to_string(message_types) + std::string(": ") + p_callback_data->pMessage;
    };

    switch (message_severity)
    {
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError: {
        core::Logger::error(LOG_CHANNEL_VULKAN, message());
    }
    break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning: {
        core::Logger::warn(LOG_CHANNEL_VULKAN, message());
    }
    break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo: {
        core::Logger::info(LOG_CHANNEL_VULKAN, message());
    }
    break;
    default: {
        core::Logger::debug(LOG_CHANNEL_VULKAN, message());
    }
    break;
    }
    return VK_FALSE;
}

vk::SurfaceKHR createSurfaceHandle(const vk::Instance instance, const SurfaceData &surface)
{
    VkSurfaceKHR vk_surface = nullptr;
#if PLATFORM == WINDOWS
    VkWin32SurfaceCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.pNext = nullptr;
    create_info.hinstance = *reinterpret_cast<HINSTANCE *>(surface.instance_handle);
    create_info.hwnd = *reinterpret_cast<HWND *>(surface.window_handle);
    vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, &vk_surface);
#elif PLATFORM == LINUX
    // TODO: Add vulkan surface creation for Linux Platform
#elif PLATFORM == MAC
    // TODO: Add vulkan surface creation for Linux Platform
#endif
    return static_cast<vk::SurfaceKHR>(vk_surface);
}

vk::PhysicalDevice selectGPU(const vk::Instance instance, const vk::SurfaceKHR surface = nullptr,
                             const GPUSelectionPreference &preference = {})
{
    const std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

    if (devices.empty())
    {
        return nullptr;
    }
    if (devices.size() == 1)
    {
        return devices[0];
    }
    std::multimap<uint32_t, vk::PhysicalDevice> scored_devices = {};
    for (const auto &device : devices)
    {
        const uint32_t score = getGPUScore(device, surface, preference);
        scored_devices.insert(std::make_pair(score, device));
    }
    return scored_devices.rbegin()->second;
}

std::vector<QueueInfo> createQueues(const vk::PhysicalDevice gpu, const vk::SurfaceKHR surface,
                                    uint32_t *out_queue_index_compute, uint32_t *out_queue_index_transfer)
{
    const auto families = getQueueFamilies(gpu, surface);
    if (families.empty())
    {
        return {};
    }
    uint32_t graphics_family_index = 0;
    uint32_t compute_family_index = 0;
    uint32_t transfer_family_index = 0;
    for (uint32_t i = 0; i < families.size(); ++i)
    {
        if ((surface != nullptr && families[i].supportsSurfaceRendering())
            || (surface == nullptr && families[i].supportsRendering()))
        {
            graphics_family_index = i;
            break;
        }
    }
    for (uint32_t i = 0; i < families.size(); ++i)
    {
        if (families[i].supportsOnlyCompute())
        {
            compute_family_index = i;
            break;
        }
    }
    for (uint32_t i = 0; i < families.size(); ++i)
    {
        if (families[i].supportsOnlyTransfer())
        {
            transfer_family_index = i;
            break;
        }
    }

    std::vector queues = {QueueInfo{"Queue_Graphics", 0, graphics_family_index, QueueFamilyFlags::GRAPHICS}};
    if (surface != nullptr)
    {
        queues[0].type = queues[0].type | QueueFamilyFlags::PRESENTATION;
    }
    if (compute_family_index != graphics_family_index)
    {
        *out_queue_index_compute = static_cast<uint32_t>(queues.size());
        queues.emplace_back(QueueInfo{"Queue_Compute", 0, compute_family_index, QueueFamilyFlags::COMPUTE});
    }
    else if (families[graphics_family_index].supportsCompute())
    {
        *out_queue_index_compute = 0;
        queues[0].type = queues[0].type | QueueFamilyFlags::COMPUTE;
    }

    if (transfer_family_index != graphics_family_index)
    {
        *out_queue_index_transfer = static_cast<uint32_t>(queues.size());
        queues.emplace_back(QueueInfo{"Queue_Transfer", 0, transfer_family_index, QueueFamilyFlags::TRANSFER});
    }
    else if (families[graphics_family_index].supportsTransfer())
    {
        *out_queue_index_transfer = 0;
        queues[0].type = queues[0].type | QueueFamilyFlags::TRANSFER;
    }
    return queues;
}

bool Device::init(const DeviceInitializationData &init_data)
{
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif
#pragma region DEBUG_MESSENGER_INFO
    auto debug_create_info = vk::DebugUtilsMessengerCreateInfoEXT{
        {},
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        // |vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
        validationCallback,
        this
    };
#pragma endregion

#pragma region CREATE_INSTANCE
    {
        std::vector<const char *> instance_layers = {};
        std::vector<const char *> instance_extensions = {};
        if (init_data.debug_mode)
        {
            instance_layers.push_back("VK_LAYER_KHRONOS_validation");
            instance_extensions.push_back(vk::EXTDebugUtilsExtensionName);
        }
        if (init_data.surface.isValid())
        {
            instance_extensions.push_back(vk::KHRSurfaceExtensionName);
            instance_extensions.push_back(PLATFORM_SURFACE_EXTENSION_NAME);
        }
        const char *app_name = init_data.app_name.c_str();
        const uint32_t version =
            VK_MAKE_VERSION(init_data.version.major, init_data.version.minor,
                            init_data.version.patch);

        vk::ApplicationInfo app_info{
            app_name, version, app_name, version,
            VK_API_VERSION_1_4
        };
        vk::InstanceCreateInfo create_info{
            {}, &app_info, instance_layers, instance_extensions
        };
        create_info.pNext = &debug_create_info;
        m_instance = vk::createInstance(create_info);

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);
#endif
    }
#pragma endregion

#pragma region CREATE_SURFACE
    if (init_data.surface.isValid())
    {
        m_surface = createSurfaceHandle(m_instance, init_data.surface);
        if (m_surface == nullptr)
        {
            core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to create surface for the current platform.");
            return false;
        }
    }
#pragma endregion

#pragma region CREATE_DEBUG_MESSENGER
    if (init_data.debug_mode)
    {
        m_debug_messenger =
            m_instance.createDebugUtilsMessengerEXT(debug_create_info);
    }
#pragma endregion

#pragma region CREATE_DEVICE_QUEUES
    {
        m_gpu = selectGPU(m_instance, m_surface, init_data.gpu_preference);

        const auto queue_infos = createQueues(m_gpu, m_surface, &m_queue_index_compute, &m_queue_index_transfer);
        if (queue_infos.empty())
        {
            core::Logger::error(LOG_CHANNEL_VULKAN, "Selected GPU does not have any queue families.");
            return false;
        }

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos = {};
        std::vector queue_priorities = {1.0f};
        for (const auto &q : queue_infos)
        {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags{0}, q.family_index, queue_priorities);
        }
        std::vector<const char *> device_extensions = getEnabledDeviceExtensions(init_data.gpu_preference.features);
        if (m_surface != nullptr)
        {
            device_extensions.push_back(vk::KHRSwapchainExtensionName);
        }
        auto create_info = vk::DeviceCreateInfo{
            vk::DeviceCreateFlags{0}, queue_create_infos, {}, device_extensions,
        };
        auto enabled_features = EnabledFeatures{};
        getEnabledFeatures(init_data.gpu_preference.features, &enabled_features);
        create_info.pNext = &enabled_features.base;

        m_device = m_gpu.createDevice(create_info);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_device);

        m_queues.reserve(queue_infos.size());
        for (const auto &q : queue_infos)
        {
            m_queues.emplace_back(Queue{m_device, q.name, q.index, q.family_index, q.type});
        }
    }
#pragma endregion

#pragma region CREATE_MEMORY_ALLOCATOR
    if (!m_memory_allocator.init(m_instance, m_gpu, m_device))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to initialize memory allocator.");
        return false;
    }
#pragma endregion
    return true;
}

void Device::destroy()
{
    if (m_memory_allocator.isValid())
    {
        m_memory_allocator.destroy();
    }
    if (m_device)
    {
        m_device.destroy();
        m_device = nullptr;
    }
    if (m_surface)
    {
        m_instance.destroySurfaceKHR(m_surface);
        m_surface = nullptr;
    }
    if (m_debug_messenger)
    {
        m_instance.destroyDebugUtilsMessengerEXT(m_debug_messenger);
        m_debug_messenger = nullptr;
    }
    if (m_instance)
    {
        m_instance.destroy();
        m_instance = nullptr;
    }
}

Buffer Device::createBuffer(const CommandEncoder &encoder, const std::string &name, const uint64_t size,
                            const void *data, const BufferUsageFlags usage) const
{
    Buffer buffer;
    if (m_device)
    {
        buffer.init(m_device, &m_memory_allocator, encoder, name, size, data, usage);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create buffer. Device is not initialized.");
    }
    return buffer;
}


Texture Device::createTexture(const std::string &name, const Size2D &size, const TextureFormat format,
                              const TextureUsageFlags usage,
                              const TextureLayout layout) const
{
    Texture texture;
    if (m_device)
    {
        texture.init(m_device, name, size, format, usage, layout, &m_memory_allocator);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create texture. Device is not initialized.");
    }
    return texture;
}

DescriptorAllocator Device::createDescriptorAllocator(const std::string &name,
                                                      const std::vector<ShaderBindingTypeRatios> &binding_type_ratios,
                                                      const uint32_t max_sets) const
{
    DescriptorAllocator allocator;
    if (m_device)
    {
        allocator.init(m_device, name, binding_type_ratios, max_sets);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Descriptor Allocator. Device is not initialized.");
    }
    return allocator;
}

DescriptorLayout Device::createDescriptorLayout(const std::string &name, const ShaderStageFlags shader_stages,
                                                const std::vector<ShaderBinding> &bindings) const
{
    DescriptorLayout layout;
    if (m_device)
    {
        layout.init(m_device, name, shader_stages, bindings);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Descriptor Layout. Device is not initialized.");
    }
    return layout;
}

Shader Device::createShader(const std::string &name, const core::Filepath &source_path,
                            const std::vector<ShaderStageFlags> &stages,
                            const std::vector<std::string> &entry_points) const
{
    Shader shader;
    if (m_device)
    {
        shader.init(m_device, name, source_path, stages, entry_points);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Shader. Device is not initialized.");
    }
    return shader;
}

PipelineLayout Device::createPipelineLayout(const std::string &name, const std::vector<DescriptorLayout> &layouts) const
{
    PipelineLayout layout;
    if (m_device)
    {
        layout.init(m_device, name, layouts);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Pipeline Layout. Device is not initialized.");
    }
    return layout;
}

PipelineCompute Device::createComputePipeline(const std::string &name, const PipelineLayout &layout,
                                              const Shader &shader) const
{
    PipelineCompute pipeline;
    if (m_device)
    {
        pipeline.init(m_device, name, layout, shader);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Compute Pipeline. Device is not initialized.");
    }
    return pipeline;
}

PipelineRender Device::createRenderPipeline(const std::string &name, const PipelineLayout &layout,
                                            const std::vector<Shader> &shaders, const RenderState &state) const
{
    PipelineRender pipeline;
    if (m_device)
    {
        pipeline.init(m_device, name, layout, shaders, state);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Render Pipeline. Device is not initialized.");
    }
    return pipeline;
}


Swapchain Device::createSwapchain(const std::string &name, const SwapchainData &data) const
{
    Swapchain swapchain;
    if (m_gpu && m_device && m_surface)
    {
        swapchain.init(m_gpu, m_device, m_surface, name, data);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Swapchain. Device or surface is not initialized.");
    }
    return swapchain;
}

CommandEncoder Device::createCommandEncoder(const std::string &name, const Queue &compatible_queue) const
{
    CommandEncoder encoder;
    if (m_device && compatible_queue.isValid())
    {
        encoder.init(m_device, name, compatible_queue.getFamilyIndex());
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Command Encoder. Device or queue is not initialized.");
    }
    return encoder;
}

Semaphore Device::createSemaphore(const std::string &name, const PipelineStageFlags stage_mask) const
{
    Semaphore semaphore;
    if (m_device)
    {
        semaphore.init(m_device, name, stage_mask);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Semaphore. Device is not initialized.");
    }
    return semaphore;
}

Fence Device::createFence(const std::string &name) const
{
    Fence fence;
    if (m_device)
    {
        fence.init(m_device, name);
    }
    else
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Failed to create Fence. Device is not initialized.");
    }
    return fence;
}

bool Device::tryReleaseTemporaryResources(const Fence &fence) const
{
    return m_memory_allocator.tryReleaseTemporaries(fence);
}

void Device::waitIdle() const
{
    m_device.waitIdle();
}

}