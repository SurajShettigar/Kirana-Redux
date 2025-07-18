// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0


#include <logger.hpp>

#include "render_delegate.hpp"

#include "common.hpp"
#include "mesh.hpp"
#include "render_pass.hpp"
#include <pxr/imaging/hd/rprim.h>
#include <pxr/imaging/hd/sprim.h>
#include <pxr/imaging/hd/bprim.h>
#include <pxr/imaging/hd/instancer.h>

using namespace pxr;
const TfTokenVector HdKiranaRenderDelegate::SUPPORTED_R_PRIM_TYPES =
{
    HdPrimTypeTokens->mesh,
};

const TfTokenVector HdKiranaRenderDelegate::SUPPORTED_S_PRIM_TYPES =
{
};

const TfTokenVector HdKiranaRenderDelegate::SUPPORTED_B_PRIM_TYPES =
{
};

HdKiranaRenderDelegate::HdKiranaRenderDelegate()
    : HdRenderDelegate()
{
    _Initialize();
}

HdKiranaRenderDelegate::HdKiranaRenderDelegate(const HdRenderSettingsMap &settings_map)
    : HdRenderDelegate(settings_map)
{
    _Initialize();
}

void HdKiranaRenderDelegate::_Initialize()
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "Initializing Kirana Render Delegate.");
    m_resource_registry = std::make_shared<HdResourceRegistry>();
}

HdKiranaRenderDelegate::~HdKiranaRenderDelegate()
{
    m_resource_registry.reset();
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "Destroying Kirana Render Delegate.");
}

const TfTokenVector &HdKiranaRenderDelegate::GetSupportedRprimTypes() const
{
    return SUPPORTED_R_PRIM_TYPES;
}

const TfTokenVector &HdKiranaRenderDelegate::GetSupportedSprimTypes() const
{
    return SUPPORTED_S_PRIM_TYPES;
}

const TfTokenVector &HdKiranaRenderDelegate::GetSupportedBprimTypes() const
{
    return SUPPORTED_B_PRIM_TYPES;
}

HdResourceRegistrySharedPtr HdKiranaRenderDelegate::GetResourceRegistry() const
{
    return m_resource_registry;
}

void HdKiranaRenderDelegate::CommitResources(HdChangeTracker *tracker)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "=> CommitResources RenderDelegate");
}

HdRenderPassSharedPtr HdKiranaRenderDelegate::CreateRenderPass(HdRenderIndex *index,
                                                               const HdRprimCollection &collection)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA,
                                "Create RenderPass with Collection: " + collection.GetName().GetString());
    return std::make_shared<HdKiranaRenderPass>(index, collection);
}

HdRprim *HdKiranaRenderDelegate::CreateRprim(const TfToken &type_id, const SdfPath &r_prim_id)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA,
                                "Create RPrim of Type: " + std::string(type_id.GetText()) + " ID: " + r_prim_id.
                                GetString());

    if (type_id == HdPrimTypeTokens->mesh)
    {
        return new HdKiranaMesh(r_prim_id);
    }
    TF_CODING_ERROR("Unknown RPrim Type: %s ID: %s", type_id.GetText(), r_prim_id.GetText());
    return nullptr;
}

void HdKiranaRenderDelegate::DestroyRprim(HdRprim *r_prim)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "Destroying RPrim with ID: " + r_prim->GetId().GetString());
    delete r_prim;
}

HdSprim *HdKiranaRenderDelegate::CreateSprim(const TfToken &type_id, const SdfPath &s_prim_id)
{
    TF_CODING_ERROR("Unknown SPrim Type: %s ID: %s", type_id.GetText(), s_prim_id.GetText());
    return nullptr;
}

HdSprim *HdKiranaRenderDelegate::CreateFallbackSprim(const TfToken &type_id)
{
    TF_CODING_ERROR("Creating unknown fallback SPrim typ: %s", type_id.GetText());
    return nullptr;
}

void HdKiranaRenderDelegate::DestroySprim(HdSprim *s_prim)
{
    TF_CODING_ERROR("Destroying SPrim is not supported: %s", s_prim->GetId());
}

HdBprim *HdKiranaRenderDelegate::CreateBprim(const TfToken &type_id, const SdfPath &b_prim_id)
{
    TF_CODING_ERROR("Unknown BPrim Type: %s ID: %s", type_id.GetText(), b_prim_id.GetText());
    return nullptr;
}

HdBprim *HdKiranaRenderDelegate::CreateFallbackBprim(const TfToken &type_id)
{
    TF_CODING_ERROR("Creating unknown fallback BPrim type: %s", type_id.GetText());
    return nullptr;
}

void HdKiranaRenderDelegate::DestroyBprim(HdBprim *b_prim)
{
    TF_CODING_ERROR("Destroying BPrim is not supported: %s", b_prim->GetId());
}

HdInstancer *HdKiranaRenderDelegate::CreateInstancer(HdSceneDelegate *delegate, const SdfPath &id)
{
    TF_CODING_ERROR("Creating Instancer is not supported %s", id.GetText());
    return nullptr;
}

void HdKiranaRenderDelegate::DestroyInstancer(HdInstancer *instancer)
{
    TF_CODING_ERROR("Destroying Instancer is not supported: %s", instancer->GetId());
}

HdRenderParam *HdKiranaRenderDelegate::GetRenderParam() const
{
    return nullptr;
}