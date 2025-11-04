// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_HYDRA_RENDER_DELEGATE_HPP
#define KIRANA_HYDRA_RENDER_DELEGATE_HPP

#include <pxr/pxr.h>
#include <pxr/imaging/hd/renderDelegate.h>


class HdKiranaRenderDelegate final : public pxr::HdRenderDelegate
{
public:
    HdKiranaRenderDelegate();
    HdKiranaRenderDelegate(const pxr::HdRenderSettingsMap &settings_map);
    ~HdKiranaRenderDelegate() override;

    HdKiranaRenderDelegate(const HdKiranaRenderDelegate &) = delete;
    HdKiranaRenderDelegate &operator =(const HdKiranaRenderDelegate &) = delete;

    /// Supported types
    [[nodiscard]] const pxr::TfTokenVector &GetSupportedRprimTypes() const override;
    [[nodiscard]] const pxr::TfTokenVector &GetSupportedSprimTypes() const override;
    [[nodiscard]] const pxr::TfTokenVector &GetSupportedBprimTypes() const override;

    [[nodiscard]] pxr::HdResourceRegistrySharedPtr GetResourceRegistry() const override;

    pxr::HdRenderPassSharedPtr
    CreateRenderPass(pxr::HdRenderIndex *index, const pxr::HdRprimCollection &collection) override;

    pxr::HdInstancer *CreateInstancer(pxr::HdSceneDelegate *delegate, const pxr::SdfPath &id) override;
    void DestroyInstancer(pxr::HdInstancer *instancer) override;

    pxr::HdRprim *CreateRprim(const pxr::TfToken &type_id, const pxr::SdfPath &r_prim_id) override;
    void DestroyRprim(pxr::HdRprim *r_prim) override;

    pxr::HdSprim *CreateSprim(const pxr::TfToken &type_id, const pxr::SdfPath &s_prim_id) override;
    pxr::HdSprim *CreateFallbackSprim(const pxr::TfToken &type_id) override;
    void DestroySprim(pxr::HdSprim *s_prim) override;

    pxr::HdBprim *CreateBprim(const pxr::TfToken &type_id, const pxr::SdfPath &b_prim_id) override;
    pxr::HdBprim *CreateFallbackBprim(const pxr::TfToken &type_id) override;
    void DestroyBprim(pxr::HdBprim *b_prim) override;

    void CommitResources(pxr::HdChangeTracker *tracker) override;

    [[nodiscard]] pxr::HdRenderParam *GetRenderParam() const override;

private:
    static const pxr::TfTokenVector SUPPORTED_R_PRIM_TYPES;
    static const pxr::TfTokenVector SUPPORTED_S_PRIM_TYPES;
    static const pxr::TfTokenVector SUPPORTED_B_PRIM_TYPES;

    void _Initialize();

    pxr::HdResourceRegistrySharedPtr m_resource_registry;
};

#endif //KIRANA_HYDRA_RENDER_DELEGATE_HPP