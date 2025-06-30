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
    m_queue = device.getPresentQueue();
    m_handle = m_device.createSwapchainKHR(create_info);

    const auto images = m_device.getSwapchainImagesKHR(m_handle);

    m_textures.reserve(num_images);
    m_swapchain_semaphores.reserve(num_images);
    m_render_semaphores.reserve(num_images);
    for (const auto &image : images)
    {
        const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{0}, image,
                                                              vk::ImageViewType::e2D,
                                                              m_format, vk::ComponentMapping{},
                                                              vk::ImageSubresourceRange{
                                                                  vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        const vk::ImageView view = m_device.createImageView(view_create_info);
        m_textures.emplace_back(Texture{m_device, image, view, data.size, data.format});

        m_swapchain_semaphores.emplace_back(m_device.createSemaphore(vk::SemaphoreCreateInfo{}));
        m_render_semaphores.emplace_back(m_device.createSemaphore(vk::SemaphoreCreateInfo{}));
    }
    return true;
}

void Swapchain::destroy()
{
    if (m_device)
    {
        if (!m_swapchain_semaphores.empty())
        {
            for (const auto &sem : m_swapchain_semaphores)
            {
                m_device.destroySemaphore(sem);
            }
            m_swapchain_semaphores.clear();
        }
        if (!m_render_semaphores.empty())
        {
            for (const auto &sem : m_render_semaphores)
            {
                m_device.destroySemaphore(sem);
            }
            m_render_semaphores.clear();
        }
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

SwapchainTexture Swapchain::getTexture() const
{

    const auto result = m_device.acquireNextImageKHR(m_handle, SWAPCHAIN_FETCH_TIMEOUT,
                                                     m_swapchain_semaphores[m_current_index]);
    if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to fetch swapchain image: " + vk::to_string(result.result));
    }
    m_swapchain_image_index = result.value;
    return {m_textures[m_swapchain_image_index], m_swapchain_semaphores[m_current_index],
            m_render_semaphores[m_current_index]};
}

void Swapchain::present() const
{
    if (const auto result = m_queue.presentKHR(
            vk::PresentInfoKHR{{m_render_semaphores[m_current_index]}, {m_handle}, {m_swapchain_image_index}});
        result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to present swapchain image: " + vk::to_string(result));
    }
    m_current_index = (m_current_index + 1) % m_textures.size();
}
}