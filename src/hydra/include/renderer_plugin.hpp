// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_HYDRA_RENDER_PLUGIN_HPP
#define KIRANA_HYDRA_RENDER_PLUGIN_HPP

#include <pxr/pxr.h>
#include <pxr/imaging/hd/rendererPlugin.h>


class HdKiranaRendererPlugin final : public pxr::HdRendererPlugin
{
public:
    HdKiranaRendererPlugin() = default;
    ~HdKiranaRendererPlugin() override = default;

    HdKiranaRendererPlugin(const HdKiranaRendererPlugin &) = delete;
    HdKiranaRendererPlugin &operator =(const HdKiranaRendererPlugin &) = delete;

    pxr::HdRenderDelegate *CreateRenderDelegate() override;

    pxr::HdRenderDelegate *CreateRenderDelegate(const pxr::HdRenderSettingsMap &settingsMap) override;

    void DeleteRenderDelegate(pxr::HdRenderDelegate *renderDelegate) override;

    [[nodiscard]] bool IsSupported(bool gpuEnabled) const override;
};

#endif //KIRANA_HYDRA_RENDER_PLUGIN_HPP