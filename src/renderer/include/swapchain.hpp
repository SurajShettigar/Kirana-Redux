// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SWAPCHAIN_HPP
#define KIRANA_RENDERER_SWAPCHAIN_HPP

#include "texture.hpp"
#include "synchronization.hpp"

namespace kirana::renderer
{
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

    [[nodiscard]] size_t getTextureCount() const
    {
        return m_textures.size();
    }

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && !m_textures.empty();
    }

    Texture &getTexture(const Semaphore &semaphore);
    SwapchainPresentInfo present() const;
private:
    SwapchainData m_data{};

    vk::Device m_device{nullptr};
    vk::SwapchainKHR m_handle{nullptr};
    std::vector<Texture> m_textures{};

    uint32_t m_swapchain_image_index{0};

    bool init(vk::PhysicalDevice gpu, vk::Device device, vk::SurfaceKHR surface, const SwapchainData &data);
};
}

#endif //KIRANA_RENDERER_SWAPCHAIN_HPP