// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "texture.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool Texture::init(const Device &device, const Size2D &size, TextureFormat format)
{

}

void Texture::destroy()
{
    if (m_device)
    {
        if (m_handle)
        {
            m_device.destroyImage(m_handle);
            m_handle = nullptr;
        }
        if (m_view)
        {
            m_device.destroyImageView(m_view);
            m_view = nullptr;
        }
    }
}

}