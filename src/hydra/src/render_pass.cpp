// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include <logger.hpp>

#include "render_pass.hpp"

#include "common.hpp"


HdKiranaRenderPass::HdKiranaRenderPass(pxr::HdRenderIndex *index, const pxr::HdRprimCollection &collection)
    : HdRenderPass(index, collection)
{

}

HdKiranaRenderPass::~HdKiranaRenderPass()
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "Destroying RenderPass");
}

void HdKiranaRenderPass::_Execute(const pxr::HdRenderPassStateSharedPtr &render_pass_state,
                                  const pxr::TfTokenVector &render_tags)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "=> Exectuting RenderPass");
}