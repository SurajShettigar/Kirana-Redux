// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_HYDRA_RENDER_PASS_HPP
#define KIRANA_HYDRA_RENDER_PASS_HPP

#include <pxr/pxr.h>
#include <pxr/imaging/hd/renderPass.h>


class HdKiranaRenderPass final : public pxr::HdRenderPass
{
public:
    HdKiranaRenderPass(pxr::HdRenderIndex *index, const pxr::HdRprimCollection &collection);
    ~HdKiranaRenderPass() override;

protected:
    void _Execute(const pxr::HdRenderPassStateSharedPtr &render_pass_state,
                  const pxr::TfTokenVector &render_tags) override;
};

#endif //KIRANA_HYDRA_RENDER_PASS_HPP