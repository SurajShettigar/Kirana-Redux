// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_MESH_HPP
#define KIRANA_SCENE_MESH_HPP

#include "common.hpp"

#include <resource_manager.hpp>

#include <span>

namespace kirana::scene
{
enum class IndexFormat : uint32_t
{
    UINT_32 = 0u,
    UINT_16 = 1u,
    UINT_8 = 2u,
};

struct IndexBufferRange
{
    IndexFormat format{};
    BufferRange range{};
};

struct IndexBuffer
{
    std::vector<uint8_t> indices_8{};
    std::vector<uint16_t> indices_16{};
    std::vector<uint32_t> indices{};

    IndexBuffer() = default;

    explicit IndexBuffer(const std::vector<uint8_t> &indices) : indices_8{indices}
    {
    }

    explicit IndexBuffer(const std::vector<uint16_t> &indices) : indices_16{indices}
    {
    }

    explicit IndexBuffer(const std::vector<uint32_t> &indices) : indices{indices}
    {
    }

    [[nodiscard]] bool isEmpty() const
    {
        return indices_8.empty() && indices_16.empty() && indices.empty();
    }

    /// Returns true if the buffer only contains indices of a single integer type (either 8, 16 or 32-bit unsigned
    /// integer).
    [[nodiscard]] bool isSingleType() const
    {
        return (!indices_8.empty() && indices.empty() && indices_16.empty()) ||
               (!indices_16.empty() && indices.empty() && indices_8.empty()) ||
               (!indices.empty() && indices_8.empty() && indices_16.empty());
    }

    [[nodiscard]] std::span<const uint8_t> getIndexRef8(const size_t offset, const size_t count) const
    {
        return offset + count > indices_8.size() ? std::span<uint8_t>{} : std::span{indices_8.data() + offset, count};
    }

    [[nodiscard]] std::span<const uint16_t> getIndexRef16(const size_t offset, const size_t count) const
    {
        return offset + count > indices_16.size() ? std::span<uint16_t>{}
                                                  : std::span{indices_16.data() + offset, count};
    }

    [[nodiscard]] std::span<const uint32_t> getIndexRef(const size_t offset, const size_t count) const
    {
        return offset + count > indices.size() ? std::span<uint32_t>{} : std::span{indices.data() + offset, count};
    }

    void extend(const IndexBuffer &rhs)
    {
        if (!rhs.indices_8.empty())
        {
            indices_8.insert(indices_8.end(), rhs.indices_8.begin(), rhs.indices_8.end());
        }
        if (!rhs.indices_16.empty())
        {
            indices_16.insert(indices_16.end(), rhs.indices_16.begin(), rhs.indices_16.end());
        }
        if (!rhs.indices.empty())
        {
            indices.insert(indices.end(), rhs.indices.begin(), rhs.indices.end());
        }
    }

    IndexBufferRange extend8(const std::vector<uint8_t> &i)
    {
        if (!i.empty())
        {
            const auto range = BufferRange{indices_8.size(), i.size()};
            indices_8.insert(indices_8.end(), i.begin(), i.end());
            return IndexBufferRange{IndexFormat::UINT_8, range};
        }
        return {};
    }

    IndexBufferRange extend16(const std::vector<uint16_t> &i)
    {
        if (!i.empty())
        {
            const auto range = BufferRange{indices_16.size(), i.size()};
            indices_16.insert(indices_16.end(), i.begin(), i.end());
            return IndexBufferRange{IndexFormat::UINT_16, range};
        }
        return {};
    }

    IndexBufferRange extend32(const std::vector<uint32_t> &i)
    {
        if (!i.empty())
        {
            const auto range = BufferRange{indices.size(), i.size()};
            indices.insert(indices.end(), i.begin(), i.end());
            return IndexBufferRange{IndexFormat::UINT_32, range};
        }
        return {};
    }
};

struct VertexBufferRange
{
    BufferRange positions{};
    BufferRange normals{};
    BufferRange uvs{};
    BufferRange colors{};
};

struct VertexBuffer
{
    std::vector<Vector3> positions{};
    std::vector<Vector3> normals{};
    std::vector<Vector2> uvs{};
    std::vector<Vector4> colors{};

    VertexBuffer() = default;

    explicit VertexBuffer(const std::vector<Vector3> &positions, const std::vector<Vector3> &normals = {},
                          const std::vector<Vector2> &uvs = {}, const std::vector<Vector4> &colors = {})
        : positions{positions}, normals{normals}, uvs{uvs}, colors{colors}
    {
    }

    explicit VertexBuffer(const std::vector<std::array<Float, 3>> &arr_positions,
                          const std::vector<std::array<Float, 3>> &arr_normals = {},
                          const std::vector<std::array<Float, 2>> &arr_uvs = {},
                          const std::vector<std::array<Float, 4>> &arr_colors = {})
    {
        positions.reserve(arr_positions.size());
        for (const auto &p : arr_positions)
        {
            positions.emplace_back(p[0], p[1], p[2]);
        }
        normals.reserve(arr_normals.size());
        for (const auto &n : arr_normals)
        {
            normals.emplace_back(n[0], n[1], n[2]);
        }
        uvs.reserve(arr_uvs.size());
        for (const auto &uv : arr_uvs)
        {
            uvs.emplace_back(uv[0], uv[1]);
        }
        colors.reserve(arr_colors.size());
        for (const auto &c : arr_colors)
        {
            colors.emplace_back(c[0], c[1], c[2], c[3]);
        }
    }

    [[nodiscard]] bool isEmpty() const
    {
        return positions.empty();
    }

    [[nodiscard]] std::span<const Vector3> getPositionRef(const BufferRange &range) const
    {
        return range.getEnd() > positions.size() ? std::span<Vector3>{}
                                                 : std::span{positions.data() + range.offset, range.size};
    }

    [[nodiscard]] std::span<const Vector3> getNormalRef(const BufferRange &range) const
    {
        return range.getEnd() > normals.size() ? std::span<Vector3>{}
                                               : std::span{normals.data() + range.offset, range.size};
    }

    [[nodiscard]] std::span<const Vector2> getUVRef(const BufferRange &range) const
    {
        return range.getEnd() > uvs.size() ? std::span<Vector2>{} : std::span{uvs.data() + range.offset, range.size};
    }

    [[nodiscard]] std::span<const Vector4> getColorRef(const BufferRange &range) const
    {
        return range.getEnd() > colors.size() ? std::span<Vector4>{}
                                              : std::span{colors.data() + range.offset, range.size};
    }

    void setPositions(const std::vector<std::array<Float, 3>> &arr_positions)
    {
        positions.clear();
        positions.reserve(arr_positions.size());
        for (const auto &p : arr_positions)
        {
            positions.emplace_back(p[0], p[1], p[2]);
        }
    }

    void setNormals(const std::vector<std::array<Float, 3>> &arr_normals)
    {
        normals.clear();
        normals.reserve(arr_normals.size());
        for (const auto &n : arr_normals)
        {
            normals.emplace_back(n[0], n[1], n[2]);
        }
    }

    void setUVs(const std::vector<std::array<Float, 2>> &arr_uvs)
    {
        uvs.clear();
        uvs.reserve(arr_uvs.size());
        for (const auto &uv : arr_uvs)
        {
            uvs.emplace_back(uv[0], uv[1]);
        }
    }

    void setColors(const std::vector<std::array<Float, 4>> &arr_colors)
    {
        colors.clear();
        colors.reserve(arr_colors.size());
        for (const auto &c : arr_colors)
        {
            colors.emplace_back(c[0], c[1], c[2], c[3]);
        }
    }

    void setColors(const std::vector<std::array<Float, 3>> &arr_colors)
    {
        colors.clear();
        colors.reserve(arr_colors.size());
        for (const auto &c : arr_colors)
        {
            colors.emplace_back(c[0], c[1], c[2], 1.0f);
        }
    }

    VertexBufferRange extend(const VertexBuffer &rhs)
    {
        VertexBufferRange result{};
        if (!rhs.positions.empty())
        {
            result.positions = BufferRange{positions.size(), rhs.positions.size()};
            positions.insert(positions.end(), rhs.positions.begin(), rhs.positions.end());
        }
        if (!rhs.normals.empty())
        {
            result.normals = BufferRange{normals.size(), rhs.normals.size()};
            normals.insert(normals.end(), rhs.normals.begin(), rhs.normals.end());
        }
        if (!rhs.uvs.empty())
        {
            result.uvs = BufferRange{uvs.size(), rhs.uvs.size()};
            uvs.insert(uvs.end(), rhs.uvs.begin(), rhs.uvs.end());
        }
        if (!rhs.colors.empty())
        {
            result.colors = BufferRange{colors.size(), rhs.colors.size()};
            colors.insert(colors.end(), rhs.colors.begin(), rhs.colors.end());
        }
        return result;
    }
};

struct Mesh final : core::IResource
{
    IndexBufferRange indices{};
    VertexBufferRange vertices{};
    MaterialHandle material{};

    Mesh() = default;

    explicit Mesh(const IndexBufferRange &indices, const VertexBufferRange &vertices,
                  const MaterialHandle &material = MaterialHandle{})
        : indices{indices}, vertices{vertices}, material{material}
    {
    }

  protected:
    bool doLoad() override
    {
        return true;
    }
    void doUnload() override
    {
    }
};
} // namespace kirana::scene

#endif // KIRANA_SCENE_MESH_HPP