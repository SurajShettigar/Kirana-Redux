// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "swapchain.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool Swapchain::init(const vk::PhysicalDevice gpu, const vk::Device device, const vk::SurfaceKHR surface,
                     const std::string &name, const SwapchainData &data)
{
    m_device = device;
    m_name = name;
    m_data = data;
    const auto extent = getExtent2D(m_data.size);
    auto format = getFormat(m_data.format);
    // TODO: Add configurable swapchain color space.
    auto color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
    auto present_mode = getPresentMode(m_data.present_mode);
    auto num_images = static_cast<uint32_t>(m_data.buffer_mode);

    const auto &caps = gpu.getSurfaceCapabilitiesKHR(surface);
    const auto &formats = gpu.getSurfaceFormatsKHR(surface);
    const auto &present_modes = gpu.getSurfacePresentModesKHR(surface);

    if (extent.width == 0 || extent.height == 0
        || extent.width < caps.minImageExtent.width
        || extent.height < caps.minImageExtent.height
        || extent.width > caps.maxImageExtent.width
        || extent.height > caps.maxImageExtent.height)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN,
                            "Current GPU does not support surface of size: " + std::to_string(extent.width) + "x" +
                            std::to_string(extent.height));
        return false;
    }
    if (caps.minImageCount > num_images || caps.maxImageCount < num_images)
    {
        num_images = std::min(std::max(num_images, caps.minImageCount), caps.maxImageCount);
        core::Logger::warn(LOG_CHANNEL_VULKAN,
                           "Restricting the swapchain image count to: " + std::to_string(num_images));
    }
    if (format == vk::Format::eUndefined)
    {
        format = formats.front().format;
        color_space = formats.front().colorSpace;
    }
    else
    {
        const auto &it = std::find_if(formats.begin(), formats.end(), [&](const auto &f) {
            return f.format == format && f.colorSpace == color_space;
        });
        if (it == formats.end())
        {
            format = formats.front().format;
            color_space = formats.front().colorSpace;
            core::Logger::warn(LOG_CHANNEL_VULKAN,
                               "Given swapchain format and/or colorspace is not supported. Switching to: " +
                               vk::to_string(format) + ", " + vk::to_string(color_space));
        }
    }

    {
        const auto &it = std::find_if(present_modes.begin(), present_modes.end(), [&](const auto &p) {
            return p == present_mode;
        });
        if (it == present_modes.end())
        {
            present_mode = present_modes.front();
            core::Logger::warn(LOG_CHANNEL_VULKAN,
                               "Given present mode is not supported. Switching to: " + vk::to_string(present_mode));
        }
    }

    const auto create_info = vk::SwapchainCreateInfoKHR{vk::SwapchainCreateFlagsKHR{0}, surface, num_images,
                                                        format,
                                                        color_space, extent, 1,
                                                        vk::ImageUsageFlagBits::eColorAttachment |
                                                        vk::ImageUsageFlagBits::eTransferDst,
                                                        vk::SharingMode::eExclusive, {}, caps.currentTransform,
                                                        vk::CompositeAlphaFlagBitsKHR::eOpaque, present_mode, true};
    m_handle = m_device.createSwapchainKHR(create_info);

    m_textures.reserve(num_images);
    const auto images = m_device.getSwapchainImagesKHR(m_handle);
    for (size_t i = 0; i < images.size(); ++i)
    {
        const auto &image = images[i];
        const std::string image_name = m_name.empty() ? "" : m_name + "_Image_" + std::to_string(i);
        const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{0}, image,
                                                              vk::ImageViewType::e2D,
                                                              format, vk::ComponentMapping{},
                                                              vk::ImageSubresourceRange{
                                                                  vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        const vk::ImageView view = m_device.createImageView(view_create_info);
        m_textures.emplace_back(Texture{m_device, image_name, image, view, data.size, data.format});
    }

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkSwapchainKHR>(m_handle));
        setDebugName(m_device, vk::ObjectType::eSwapchainKHR, handle, m_name);
    }
    return true;
}

void Swapchain::destroy()
{
    if (m_device)
    {
        if (!m_textures.empty())
        {
            for (const auto &tex : m_textures)
            {
                m_device.destroyImageView(tex.m_view);
            }
            m_textures.clear();
        }
        if (m_handle)
        {
            m_device.destroySwapchainKHR(m_handle);
            m_handle = nullptr;
        }
    }
}

Texture &Swapchain::getTexture(const Semaphore &semaphore)
{
    const auto result = m_device.acquireNextImageKHR(m_handle, SWAPCHAIN_FETCH_TIMEOUT,
                                                     semaphore.m_handle);
    if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to fetch swapchain image: " + vk::to_string(result.result));
    }
    m_swapchain_image_index = result.value;
    return m_textures[m_swapchain_image_index];
}

SwapchainPresentInfo Swapchain::present() const
{
    return SwapchainPresentInfo{m_handle, m_swapchain_image_index};
}
}