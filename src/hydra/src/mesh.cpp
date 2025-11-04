// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include <logger.hpp>

#include "mesh.hpp"

#include "common.hpp"


HdKiranaMesh::HdKiranaMesh(const pxr::SdfPath &id)
    : HdMesh(id)
{
}

pxr::HdDirtyBits HdKiranaMesh::GetInitialDirtyBitsMask() const
{
    return pxr::HdChangeTracker::Clean | pxr::HdChangeTracker::DirtyTransform;
}

pxr::HdDirtyBits HdKiranaMesh::_PropagateDirtyBits(const pxr::HdDirtyBits bits) const
{
    return bits;
}

void HdKiranaMesh::_InitRepr(const pxr::TfToken &repr_token, pxr::HdDirtyBits *dirty_bits)
{

}

void HdKiranaMesh::Sync(pxr::HdSceneDelegate *delegate, pxr::HdRenderParam *render_param, pxr::HdDirtyBits *dirty_bits,
                        const pxr::TfToken &repr_token)
{
    kirana::core::Logger::debug(LOG_CHANNEL_HYDRA, "* (multi-threaded) Syncing Mesh: " + GetId().GetString());
}