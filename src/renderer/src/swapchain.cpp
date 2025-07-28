// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "swapchain.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
inline vk::Extent2D getSwapchainExtent(const vk::SurfaceCapabilitiesKHR &caps, const Size2D &size)
{
    auto extent = getExtent2D(size);
    if (caps.currentExtent.width == std::numeric_limits<uint32_t>::max() && caps.currentExtent.height ==
        std::numeric_limits<uint32_t>::max())
    {
        extent = vk::Extent2D{std::clamp(extent.width, caps.minImageExtent.width, caps.maxImageExtent.width),
                              std::clamp(extent.height, caps.minImageExtent.height, caps.maxImageExtent.height)};
    }
    else
    {
        extent = caps.currentExtent;
    }
    return extent;
}

bool Swapchain::init()
{
    VkSwapchainKHR vk_handle = nullptr;
    const auto result = vkCreateSwapchainKHR(
        m_device, reinterpret_cast<const VkSwapchainCreateInfoKHR *>(&m_create_info), nullptr, &vk_handle);

    if (result != VK_SUCCESS)
    {
        return false;
    }
    m_handle = vk_handle;

    const std::vector<vk::Image> images = m_device.getSwapchainImagesKHR(m_handle);
    const auto num_images = images.size();
    m_textures.reserve(num_images);
    for (size_t i = 0; i < num_images; ++i)
    {
        const auto &image = images[i];
        const std::string image_name = m_name.empty() ? "" : m_name + "_Image_" + std::to_string(i);
        const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{0}, image,
                                                              vk::ImageViewType::e2D,
                                                              m_create_info.imageFormat, vk::ComponentMapping{},
                                                              vk::ImageSubresourceRange{
                                                                  vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        const vk::ImageView view = m_device.createImageView(view_create_info);
        m_textures.emplace_back(Texture{m_device, image_name, image, view, m_data.size, m_data.format});
    }

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkSwapchainKHR>(m_handle));
        setDebugName(m_device, vk::ObjectType::eSwapchainKHR, handle, m_name);
    }

    m_semaphore_index = 0;
    m_swapchain_image_index = 0;
    return true;
}

bool Swapchain::init(const vk::PhysicalDevice gpu, const vk::Device device, const vk::SurfaceKHR surface,
                     const std::string &name, const SwapchainData &data)
{
    m_name = name;
    m_data = data;

    m_gpu = gpu;
    m_device = device;
    m_surface = surface;

    auto format = getFormat(m_data.format);
    // TODO: Add configurable swapchain color space.
    auto color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
    auto present_mode = getPresentMode(m_data.present_mode);
    auto num_images = static_cast<uint32_t>(m_data.buffer_mode);

    const auto &caps = m_gpu.getSurfaceCapabilitiesKHR(m_surface);
    const auto &formats = m_gpu.getSurfaceFormatsKHR(m_surface);
    const auto &present_modes = m_gpu.getSurfacePresentModesKHR(m_surface);

    const auto extent = getSwapchainExtent(caps, m_data.size);

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

    m_semaphores.reserve(num_images);
    for (size_t i = 0; i < num_images; ++i)
    {
        m_semaphores.emplace_back();
        const std::string semaphore_name = "Semaphore_" + m_name.empty() ? "" : m_name + "_" + std::to_string(i);
        m_semaphores.back().init(m_device, semaphore_name, PipelineStageFlags::TOP_OF_PIPE);
    }

    m_create_info = vk::SwapchainCreateInfoKHR{vk::SwapchainCreateFlagsKHR{0}, m_surface, num_images,
                                               format,
                                               color_space, extent, 1,
                                               vk::ImageUsageFlagBits::eColorAttachment |
                                               vk::ImageUsageFlagBits::eTransferDst,
                                               vk::SharingMode::eExclusive, {}, caps.currentTransform,
                                               vk::CompositeAlphaFlagBitsKHR::eOpaque, present_mode, true};
    return init();
}

void Swapchain::destroy()
{
    if (m_device)
    {
        if (!m_semaphores.empty())
        {
            for (auto &s : m_semaphores)
            {
                s.destroy();
            }
            m_semaphores.clear();

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

void Swapchain::resize(const Size2D &size)
{
    if (size.width == m_data.size.width && size.height == m_data.size.height)
    {
        return;
    }

    m_data.size = size;
    m_create_info.setImageExtent(getSwapchainExtent(m_gpu.getSurfaceCapabilitiesKHR(m_surface), size));
}

bool Swapchain::reinitialize()
{
    if (!m_data.size.isValid() || !m_device || !m_surface)
    {
        return false;
    }
    m_device.waitIdle();
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
    return init();
}

std::optional<SwapchainTextureInfo> Swapchain::getTexture()
{
    if (!isValid())
    {
        return {};
    }
    const auto result = m_device.acquireNextImageKHR(m_handle, SWAPCHAIN_FETCH_TIMEOUT,
                                                     m_semaphores[m_semaphore_index].getNativeHandle());
    if (result.result == vk::Result::eErrorOutOfDateKHR)
    {
        if (!reinitialize())
        {
            return {};
        }
    }
    else if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to fetch swapchain image: " + vk::to_string(result.result));
        return {};
    }
    m_swapchain_image_index = result.value;
    const auto info = SwapchainTextureInfo{m_swapchain_image_index, m_textures[m_swapchain_image_index],
                                           m_semaphores[m_semaphore_index]};
    m_semaphore_index = (m_semaphore_index + 1) % m_semaphores.size();
    return info;
}

SwapchainPresentInfo Swapchain::present() const
{
    return SwapchainPresentInfo{m_handle, m_swapchain_image_index};
}
}