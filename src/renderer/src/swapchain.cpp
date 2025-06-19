// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "swapchain.hpp"

#include <logger.hpp>

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{


bool Swapchain::init(const Device &device, const SwapchainData &data)
{
    if (!device.isValid() || device.m_surface == nullptr)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Device or surface is not initialized. Swapchain can only be created once device and surface is initialized");
        return false;
    }

    m_size = getExtent2D(data.size);
    m_format = getFormat(data.format);
    // TODO: Add configurable swapchain color space.
    m_color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
    m_present_mode = getPresentMode(data.present_mode);
    auto num_images = static_cast<uint32_t>(data.buffer_mode);

    const auto &caps = device.m_gpu.getSurfaceCapabilitiesKHR(device.m_surface);
    const auto &formats = device.m_gpu.getSurfaceFormatsKHR(device.m_surface);
    const auto &present_modes = device.m_gpu.getSurfacePresentModesKHR(device.m_surface);

    if (m_size.width == 0 || m_size.height == 0
        || m_size.width < caps.minImageExtent.width
        || m_size.height < caps.minImageExtent.height
        || m_size.width > caps.maxImageExtent.width
        || m_size.height > caps.maxImageExtent.height)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Current GPU does not support surface of size: " + std::to_string(m_size.width) + "x" +
                            std::to_string(m_size.height));
        return false;
    }
    if (caps.minImageCount > num_images || caps.maxImageCount < num_images)
    {
        num_images = std::min(std::max(num_images, caps.minImageCount), caps.maxImageCount);
        core::Logger::warn(LOG_CHANNEL_VULKAN,
                           "Restricting the swapchain image count to: " + std::to_string(num_images));
    }
    if (m_format == vk::Format::eUndefined)
    {
        m_format = formats.front().format;
        m_color_space = formats.front().colorSpace;
    }
    else
    {
        const auto &it = std::find_if(formats.begin(), formats.end(), [&](const auto &f) {
            return f.format == m_format && f.colorSpace == m_color_space;
        });
        if (it == formats.end())
        {
            m_format = formats.front().format;
            m_color_space = formats.front().colorSpace;
            core::Logger::warn(LOG_CHANNEL_VULKAN,
                               "Given swapchain format and/or colorspace is not supported. Switching to: " +
                               vk::to_string(m_format) + ", " + vk::to_string(m_color_space));
        }
    }

    {
        const auto &it = std::find_if(present_modes.begin(), present_modes.end(), [&](const auto &p) {
            return p == m_present_mode;
        });
        if (it == present_modes.end())
        {
            m_present_mode = present_modes.front();
            core::Logger::warn(LOG_CHANNEL_VULKAN,
                               "Given present mode is not supported. Switching to: " + vk::to_string(m_present_mode));
        }
    }

    const auto create_info = vk::SwapchainCreateInfoKHR{vk::SwapchainCreateFlagsKHR{0}, device.m_surface, num_images,
                                                        m_format,
                                                        m_color_space, m_size, 1,
                                                        vk::ImageUsageFlagBits::eColorAttachment |
                                                        vk::ImageUsageFlagBits::eTransferDst,
                                                        vk::SharingMode::eExclusive, {}, caps.currentTransform,
                                                        vk::CompositeAlphaFlagBitsKHR::eOpaque, m_present_mode, true};

    m_device = device.m_device;
    m_handle = m_device.createSwapchainKHR(create_info);

    m_images = m_device.getSwapchainImagesKHR(m_handle);
    for (const auto &image : m_images)
    {
        const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{0}, image,
                                                              vk::ImageViewType::e2D,
                                                              m_format, vk::ComponentMapping{},
                                                              vk::ImageSubresourceRange{
                                                                  vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        vk::ImageView view = m_device.createImageView(view_create_info);
        m_image_views.emplace_back(std::move(view));
    }
    return true;
}

void Swapchain::destroy()
{
    if (m_device)
    {
        if (!m_image_views.empty())
        {
            for (const auto &view : m_image_views)
            {
                m_device.destroyImageView(view);
            }
            m_image_views.clear();
        }
        if (m_handle)
        {
            m_device.destroySwapchainKHR(m_handle);
            m_handle = nullptr;
        }
    }
}
}