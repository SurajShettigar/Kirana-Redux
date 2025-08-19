// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_COMMON_HPP
#define KIRANA_SCENE_COMMON_HPP

#include <handle.hpp>

#include <string>
#include <array>

namespace kirana::scene
{
const std::string LOG_CHANNEL_SCENE{"SCENE"};

const std::string DEFAULT_NAME_SCENE{"Scene"};
const std::string DEFAULT_NAME_NODE{"Scene"};
const std::string DEFAULT_NAME_CAMERA{"Camera"};
const std::string DEFAULT_NAME_MESH{"Mesh"};
const std::string DEFAULT_NAME_MATERIAL{"Material"};

using FLOAT = float;
using VECTOR_2 = std::array<FLOAT, 2>;
using VECTOR_3 = std::array<FLOAT, 3>;
using VECTOR_4 = std::array<FLOAT, 4>;
using MATRIX_3 = std::array<VECTOR_3, 3>;
using MATRIX_4 = std::array<VECTOR_4, 4>;

static constexpr MATRIX_4 IDENTITY_MATRIX_4 = {VECTOR_4{1.0, 0.0, 0.0, 0.0},
                                               VECTOR_4{0.0, 1.0, 0.0, 0.0},
                                               VECTOR_4{0.0, 0.0, 1.0, 0.0},
                                               VECTOR_4{0.0, 0.0, 0.0, 1.0}};

struct BufferRange
{
    size_t offset{};
    size_t size{};

    [[nodiscard]] bool isValid() const
    {
        return size > 0;
    }

    [[nodiscard]] size_t getEnd() const
    {
        return offset + size;
    }
};

struct NodeTag
{
};

struct HierarchyTransformTag
{
};

struct CameraTag
{
};

struct LightTag
{
};

struct MaterialTag
{
};

struct MeshTag
{
};

using NodeHandle = core::Handle<NodeTag>;
using HierarchyTransformHandle = core::Handle<HierarchyTransformTag>;
using CameraHandle = core::Handle<CameraTag>;
using LightHandle = core::Handle<LightTag>;
using MaterialHandle = core::Handle<MaterialTag>;
using MeshHandle = core::Handle<MeshTag>;
}

#endif //KIRANA_SCENE_COMMON_HPP