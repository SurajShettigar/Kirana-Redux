// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SWAPCHAIN_HPP
#define KIRANA_RENDERER_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <no_copy.hpp>

#include "device.hpp"

namespace kirana::renderer
{
class Swapchain : core::NoCopy
{
public:
    Swapchain() = default;
    ~Swapchain()
    {
        destroy();
    }

    bool init(const Device &device, const SwapchainData &data);
    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && m_images.size() > 0 && m_image_views.size() > 0;
    }

private:
    vk::Extent2D m_size{};
    vk::Format m_format{};
    vk::ColorSpaceKHR m_color_space{};
    vk::PresentModeKHR m_present_mode{};

    vk::Device m_device{nullptr};
    vk::SwapchainKHR m_handle{nullptr};
    std::vector<vk::Image> m_images{};
    std::vector<vk::ImageView> m_image_views{};
};
}

#endif //KIRANA_RENDERER_SWAPCHAIN_HPP