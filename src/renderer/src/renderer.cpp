// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "renderer.hpp"

namespace kirana::renderer
{
bool Renderer::init(const DeviceInitializationData &init_data)
{
    return m_device.init(init_data);
}

void Renderer::update()
{
}

void Renderer::render()
{
}

void Renderer::lateUpdate()
{
}

void Renderer::clean()
{
    m_device.destroy();
}
}