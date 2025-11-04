// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SWAPCHAIN_HPP
#define KIRANA_RENDERER_SWAPCHAIN_HPP

#include "texture.hpp"
#include "synchronization.hpp"

#include <optional>

namespace kirana::renderer
{
struct SwapchainTextureInfo
{
    uint32_t texture_index;
    Texture &texture;
    const Semaphore &semaphore;
};

struct SwapchainPresentInfo
{
    vk::SwapchainKHR handle;
    uint32_t image_index;
};

class Swapchain
{
    friend class Device;

public:
    Swapchain() = default;
    ~Swapchain() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && m_data.size.isValid() && !m_textures.empty();
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] size_t getTextureCount() const
    {
        return m_textures.size();
    }

    void resize(const Size2D &size);
    bool reinitialize();

    std::optional<SwapchainTextureInfo> getTexture();
    [[nodiscard]] SwapchainPresentInfo present() const;
private:
    std::string m_name{};
    SwapchainData m_data{};

    vk::PhysicalDevice m_gpu{nullptr};
    vk::Device m_device{nullptr};
    vk::SurfaceKHR m_surface{nullptr};

    vk::SwapchainCreateInfoKHR m_create_info{};
    vk::SwapchainKHR m_handle{nullptr};
    std::vector<Texture> m_textures{};
    std::vector<Semaphore> m_semaphores{};

    uint32_t m_semaphore_index{0};
    uint32_t m_swapchain_image_index{0};

    bool init(vk::PhysicalDevice gpu, vk::Device device, vk::SurfaceKHR surface, const std::string &name,
              const SwapchainData &data);
    bool init();
};
}

#endif //KIRANA_RENDERER_SWAPCHAIN_HPP