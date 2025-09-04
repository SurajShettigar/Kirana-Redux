// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_COMMON_HPP
#define KIRANA_SCENE_COMMON_HPP

#include <handle.hpp>
#include <vector2.hpp>
#include <quaternion.hpp>

#include <string>

namespace kirana::scene
{
const std::string LOG_CHANNEL_SCENE{"SCENE"};

const std::string DEFAULT_NAME_SCENE{"Scene"};
const std::string DEFAULT_NAME_NODE{"Node"};
const std::string DEFAULT_NAME_CAMERA{"Camera"};
const std::string DEFAULT_NAME_MESH{"Mesh"};
const std::string DEFAULT_NAME_MATERIAL{"Material"};

using Float = float;
using Vector2 = ganita::Vector2;
using Vector3 = ganita::Vector3;
using Vector4 = ganita::Vector4;
using Matrix4 = ganita::Matrix4x4;
using Quaternion = ganita::Quaternion;

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

struct ImageTag
{
};

struct TextureTag
{
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

using ImageHandle = core::Handle<ImageTag>;
using TextureHandle = core::Handle<TextureTag>;
using NodeHandle = core::Handle<NodeTag>;
using HierarchyTransformHandle = core::Handle<HierarchyTransformTag>;
using CameraHandle = core::Handle<CameraTag>;
using LightHandle = core::Handle<LightTag>;
using MaterialHandle = core::Handle<MaterialTag>;
using MeshHandle = core::Handle<MeshTag>;
}

#endif //KIRANA_SCENE_COMMON_HPP