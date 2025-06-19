// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_RENDERER_HPP
#define KIRANA_RENDERER_RENDERER_HPP

#include "common.hpp"
#include "device.hpp"

namespace kirana::renderer
{
class Renderer : core::NoCopy
{
public:
    Renderer() = default;
    ~Renderer() = default;


    Renderer(const Renderer &renderer) = delete;
    Renderer &operator=(const Renderer &renderer) = delete;

    bool init(const DeviceInitializationData &init_data);
    void update();
    void render();
    void lateUpdate();
    void clean();

    [[nodiscard]] const Device &getDevice() const
    {
        return m_device;
    }

private:
    Device m_device;
};
}

#endif //KIRANA_RENDERER_RENDERER_HPP