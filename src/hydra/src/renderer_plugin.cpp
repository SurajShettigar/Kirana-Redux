// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include <logger.hpp>

#include "renderer_plugin.hpp"

#include "common.hpp"
#include "render_delegate.hpp"

#include <pxr/imaging/hd/rendererPluginRegistry.h>

using namespace pxr;
// Register the plugin with the renderer plugin system.
TF_REGISTRY_FUNCTION(TfType)
{
    HdRendererPluginRegistry::Define<HdKiranaRendererPlugin>();
}


HdRenderDelegate *HdKiranaRendererPlugin::CreateRenderDelegate()
{
    kirana::core::Logger::get().init(LOG_CHANNEL_HYDRA, kirana::core::Logger::Level::L_DEBUG);
    return new HdKiranaRenderDelegate();
}

HdRenderDelegate *HdKiranaRendererPlugin::CreateRenderDelegate(const HdRenderSettingsMap &settingsMap)
{
    kirana::core::Logger::get().init(LOG_CHANNEL_HYDRA, kirana::core::Logger::Level::L_DEBUG);
    return new HdKiranaRenderDelegate(settingsMap);
}

void HdKiranaRendererPlugin::DeleteRenderDelegate(HdRenderDelegate *renderDelegate)
{
    delete renderDelegate;
}

bool HdKiranaRendererPlugin::IsSupported(bool gpuEnabled) const
{
    return true;
}