// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_HYDRA_MESH_HPP
#define KIRANA_HYDRA_MESH_HPP

#include <pxr/pxr.h>
#include <pxr/imaging/hd/mesh.h>

class HdKiranaMesh final : public pxr::HdMesh
{
public:
    HF_MALLOC_TAG_NEW("new HdKiranaMesh");

    explicit HdKiranaMesh(const pxr::SdfPath &id);
    ~HdKiranaMesh() override = default;

    HdKiranaMesh(const HdKiranaMesh &) = delete;
    HdKiranaMesh &operator =(const HdKiranaMesh &) = delete;

    pxr::HdDirtyBits GetInitialDirtyBitsMask() const override;

    void Sync(pxr::HdSceneDelegate *delegate, pxr::HdRenderParam *render_param, pxr::HdDirtyBits *dirty_bits,
              const pxr::TfToken &repr_token) override;

protected:
    void _InitRepr(const pxr::TfToken &repr_token, pxr::HdDirtyBits *dirty_bits) override;

    pxr::HdDirtyBits _PropagateDirtyBits(pxr::HdDirtyBits bits) const override;
};

#endif //KIRANA_HYDRA_MESH_HPP