// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SWAPCHAIN_HPP
#define KIRANA_RENDERER_SWAPCHAIN_HPP

#include "texture.hpp"

namespace kirana::renderer
{
struct SwapchainTexture
{
    Texture texture{};
    vk::Semaphore m_wait_semaphore{};
    vk::Semaphore m_signal_semaphore{};
};

class Swapchain
{
public:
    Swapchain() = default;
    ~Swapchain() = default;

    bool init(const Device &device, const SwapchainData &data);
    void destroy();

    [[nodiscard]] size_t getTextureCount() const
    {
        return m_textures.size();
    }

    SwapchainTexture getTexture() const;
    void present() const;

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && !m_textures.empty();
    }

private:
    vk::Extent2D m_size{};
    vk::Format m_format{};
    vk::ColorSpaceKHR m_color_space{};
    vk::PresentModeKHR m_present_mode{};

    vk::Device m_device{nullptr};
    vk::Queue m_queue{nullptr};
    vk::SwapchainKHR m_handle{nullptr};
    std::vector<Texture> m_textures{};

    std::vector<vk::Semaphore> m_swapchain_semaphores{};
    std::vector<vk::Semaphore> m_render_semaphores{};

    mutable size_t m_current_index{0};
    mutable uint32_t m_swapchain_image_index{0};
};
}

#endif //KIRANA_RENDERER_SWAPCHAIN_HPP