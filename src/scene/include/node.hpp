// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_NODE_HPP
#define KIRANA_SCENE_NODE_HPP

#include "common.hpp"

#include <resource_manager.hpp>

#include <variant>
#include <optional>

namespace kirana::scene
{

enum class NodeType: uint8_t
{
    EMPTY = 0u,
    CAMERA = 1u,
    LIGHT = 2u,
    MESH = 3u,
    COUNT = 4u,
};

enum class NodeFlags: uint32_t
{
    NONE = 0u,
    HIDDEN = 1u << 0u,
    SELECTED = 1u << 1u,
};

/// Scene hierarchy is represented through a binary tree of nodes. We use the left-child, right sibling method to store
/// a k-ary scene hierarchy.
struct Node final : core::IResource
{
    NodeFlags flags{NodeFlags::NONE};
    std::optional<NodeHandle> parent{std::nullopt};
    std::optional<NodeHandle> first_child{std::nullopt};
    std::optional<NodeHandle> next_sibling{std::nullopt};
    uint32_t level{0};

    std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> resource{std::nullopt};

    [[nodiscard]] NodeType getNodeType() const
    {
        return resource ? static_cast<NodeType>(resource.value().index() + 1) : NodeType::EMPTY;
    }

    Node() = default;

    explicit Node(const NodeFlags flags, const std::optional<NodeHandle> &parent = std::nullopt,
                  const std::optional<NodeHandle> &first_child = std::nullopt,
                  const std::optional<NodeHandle> &next_sibling = std::nullopt, const uint32_t level = 0,
                  const std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> &resource = std::nullopt)
        : IResource{}, flags{flags}, parent{parent}, first_child{first_child}, next_sibling{next_sibling}, level{level},
          resource{resource}
    {
    }
};
}

#endif //KIRANA_SCENE_NODE_HPP