// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DEVICE_HPP
#define KIRANA_RENDERER_DEVICE_HPP

#include "queue.hpp"
#include "swapchain.hpp"
#include "command_encoder.hpp"
#include "descriptor_allocator.hpp"
#include "pipeline_layout.hpp"
#include "pipeline_compute.hpp"
#include "pipeline_render.hpp"
#include "resource_allocator.hpp"

namespace kirana::renderer
{
class Device
{
    friend VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity, vk::DebugUtilsMessageTypeFlagsEXT message_types,
        const vk::DebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data);

  public:
    Device() = default;
    ~Device() = default;

    bool init(const DeviceInitializationData &init_data);
    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_instance != nullptr && m_device != nullptr && !m_queues.empty();
    }

    [[nodiscard]] vk::Instance getNativeInstance() const
    {
        return m_instance;
    }

    [[nodiscard]] vk::PhysicalDevice getNativeGPUHandle() const
    {
        return m_gpu;
    }

    [[nodiscard]] const GPUFeatures &getFeatures() const
    {
        return m_features;
    }

    [[nodiscard]] const GPULimits &getLimits() const
    {
        return m_limits;
    }

    [[nodiscard]] bool supportsAsyncCompute() const
    {
        return m_queue_index_compute > 0;
    }

    [[nodiscard]] bool supportsAsyncTransfer() const
    {
        return m_queue_index_transfer > 0;
    }

    [[nodiscard]] vk::Device getNativeHandle() const
    {
        return m_device;
    }

    [[nodiscard]] const Queue &getGraphicsQueue() const
    {
        return m_queues[0];
    }

    [[nodiscard]] Queue &getGraphicsQueue()
    {
        return m_queues[0];
    }

    [[nodiscard]] const Queue &getPresentQueue() const
    {
        return m_queues[0];
    }

    [[nodiscard]] Queue &getPresentQueue()
    {
        return m_queues[0];
    }

    [[nodiscard]] const Queue &getComputeQueue() const
    {
        return m_queues[m_queue_index_compute];
    }

    [[nodiscard]] Queue &getComputeQueue()
    {
        return m_queues[m_queue_index_compute];
    }

    [[nodiscard]] const Queue &getTransferQueue() const
    {
        return m_queues[m_queue_index_transfer];
    }

    [[nodiscard]] Queue &getTransferQueue()
    {
        return m_queues[m_queue_index_transfer];
    }

    /// Before any resource creation or writing data from host, this function needs to be called to initialize command
    /// buffer.
    void beginAllocation();
    /// After any resource creation or writing data from host, this function needs to be called to finalize command
    /// buffer and submit it to a queue.
    void endAllocation();

    BufferHandle createBuffer(const std::string &name, uint64_t size, BufferUsageFlags usage,
                              const void *data = nullptr, ResourceMemoryType memory_type = ResourceMemoryType::AUTO,
                              ResourceMemoryFlags memory_flags =
                                  ResourceMemoryFlags::HOST_ACCESS_SEQUENTIAL_WRITE | ResourceMemoryFlags::MAPPED |
                                  ResourceMemoryFlags::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD);
    const Buffer *getBuffer(BufferHandle handle) const;
    bool writeBuffer(BufferHandle handle, uint64_t size, const void *data);
    void destroyBuffer(BufferHandle handle);

    TextureHandle createTexture(const std::string &name, const Size2D &size, TextureFormat format,
                                TextureUsageFlags usage, TextureLayout layout = TextureLayout::GENERAL,
                                const void *data = nullptr, ResourceMemoryType memory_type = ResourceMemoryType::DEVICE,
                                ResourceMemoryFlags memory_flags = ResourceMemoryFlags::UNKNOWN);
    const Texture *getTexture(TextureHandle handle) const;
    bool writeImage(TextureHandle handle, const void *data);
    void destroyTexture(TextureHandle handle);

    TextureSamplerHandle createTextureSampler(const std::string &name,
                                                            const SamplerData &data = SamplerData{});
    const TextureSampler *getTextureSampler(TextureSamplerHandle handle) const;
    void destroyTextureSampler(TextureSamplerHandle handle);

    [[nodiscard]] DescriptorLayout createDescriptorLayout(const std::string &name, ShaderStageFlags shader_stages,
                                                          const std::vector<ShaderBinding> &bindings = {}) const;

    [[nodiscard]] DescriptorSet allocateDescriptorSet(
        const std::string &name, const DescriptorLayout &layout,
        const std::vector<ShaderBindingResource> &binding_resources = {}) const;

    [[nodiscard]] Shader createShader(const std::string &name, const core::Filepath &source_path,
                                      const std::vector<ShaderStageFlags> &stages = {ShaderStageFlags::COMPUTE},
                                      const std::vector<std::string> &entry_points = {"main"}) const;

    [[nodiscard]] PipelineLayout createPipelineLayout(const std::string &name,
                                                      const std::vector<DescriptorLayout> &layouts) const;

    [[nodiscard]] PipelineCompute createComputePipeline(const std::string &name, const PipelineLayout &layout,
                                                        const Shader &shader) const;

    [[nodiscard]] PipelineRender createRenderPipeline(const std::string &name, const PipelineLayout &layout,
                                                      const std::vector<Shader> &shaders,
                                                      const RenderState &state = {}) const;

    [[nodiscard]] Swapchain createSwapchain(const std::string &name, const SwapchainData &data) const;

    [[nodiscard]] CommandEncoder createCommandEncoder(const std::string &name, const Queue &compatible_queue) const;

    [[nodiscard]] Semaphore createSemaphore(const std::string &name,
                                            PipelineStageFlags stage_mask = PipelineStageFlags::TOP_OF_PIPE) const;

    [[nodiscard]] Fence createFence(const std::string &name) const;

    void waitIdle() const;

  private:
    vk::Instance m_instance{nullptr};
    vk::DebugUtilsMessengerEXT m_debug_messenger{nullptr};

    vk::SurfaceKHR m_surface{nullptr};

    vk::PhysicalDevice m_gpu{nullptr};
    vk::Device m_device{nullptr};

    GPUFeatures m_features{};
    GPULimits m_limits{};

    std::vector<Queue> m_queues{};
    uint32_t m_queue_index_compute{0};
    uint32_t m_queue_index_transfer{0};

    ResourceAllocator m_resource_allocator{};
    DescriptorAllocator m_descriptor_allocator{};

    bool m_allocation_started{false};
};
} // namespace kirana::renderer
#endif // KIRANA_RENDERER_DEVICE_HPP