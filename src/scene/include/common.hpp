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
const std::string DEFAULT_NAME_IMAGE{"Image"};
const std::string DEFAULT_NAME_TEXTURE{"Texture"};
const std::string DEFAULT_NAME_NODE{"Node"};
const std::string DEFAULT_NAME_CAMERA{"Camera"};
const std::string DEFAULT_NAME_LIGHT{"Light"};
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

struct Camera;
using CameraHandle = core::Handle<Camera>;
class Image;
using ImageHandle = core::Handle<Image>;
struct PunctualLight;
using PunctualLightHandle = core::Handle<PunctualLight>;
struct MaterialPBR;
using MaterialHandle = core::Handle<MaterialPBR>;
struct Mesh;
using MeshHandle = core::Handle<Mesh>;
struct Node;
using NodeHandle = core::Handle<Node>;
class Scene;
using SceneHandle = core::Handle<Scene>;
struct Texture;
using TextureHandle = core::Handle<Texture>;
struct HierarchyTransform;
using HierarchyTransformHandle = core::Handle<HierarchyTransform>;

} // namespace kirana::scene

#endif // KIRANA_SCENE_COMMON_HPP