// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_TRANSFORM_HPP
#define KIRANA_SCENE_TRANSFORM_HPP

#include "common.hpp"

#include <resource_manager.hpp>

namespace kirana::scene
{
class Transform
{
public:
    Transform() = default;

    explicit Transform(const MATRIX_4 &matrix)
        : m_local_to_world{matrix}
    {
        // TODO: Calculate matrix inverse for the world-to-local matrix.
    }

    Transform operator *(const Transform &rhs) const
    {
        // TODO: Add transform matrix multiplication.
        return Transform{};
        // return Transform{lhs.m_local_to_world * rhs.m_local_to_world};
    }

private:
    MATRIX_4 m_local_to_world{IDENTITY_MATRIX_4};
    MATRIX_4 m_world_to_local{IDENTITY_MATRIX_4};
};

struct HierarchyTransform final : core::IResource
{
    /// Local transform in a scene hierarchy.
    Transform local{};
    /// World transform in a scene hierarchy. (Obtained by chain multiplication of world transforms from root to the
    /// current local transform).
    Transform world{};

    HierarchyTransform() = default;

    explicit HierarchyTransform(const Transform &local, const Transform &world)
        : IResource{}, local{local}, world{world}
    {
    }
};
}

#endif //KIRANA_SCENE_TRANSFORM_HPP