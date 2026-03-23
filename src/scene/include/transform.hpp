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

    explicit Transform(const Matrix4 &matrix)
        : m_local_to_world{matrix}, m_world_to_local{m_local_to_world.transformInverse()}
    {
        decomposeMatrix();
    }

    explicit Transform(const std::array<Float, 16> &matrix)
        : m_local_to_world{matrix}, m_world_to_local{m_local_to_world.transformInverse()}
    {
        decomposeMatrix();
    }

    explicit Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale)
        : m_position{position}, m_rotation{rotation}, m_scale{scale}
    {
        updateMatrix();
    }

    explicit Transform(const Vector3 &position, const Vector3 &euler_angles, const Vector3 &scale)
        : m_position{position}, m_rotation{Quaternion::fromEulerAngles(euler_angles)}, m_scale{scale}
    {
        updateMatrix();
    }

    explicit Transform(const std::array<float, 3> &position, const std::array<float, 4> &quaternion,
                       const std::array<float, 3> &scale)
        : m_position{position}, m_rotation{quaternion}, m_scale{scale}
    {
        updateMatrix();
    }

    explicit Transform(const std::array<float, 3> &position, const std::array<float, 3> &euler_angles,
                       const std::array<float, 3> &scale)
        : m_position{position}, m_rotation{Quaternion::fromEulerAngles(Vector3{euler_angles})}, m_scale{scale}
    {
        updateMatrix();
    }

    [[nodiscard]] const Vector3 &getPosition() const
    {
        return m_position;
    }

    [[nodiscard]] const Quaternion &getRotation() const
    {
        return m_rotation;
    }

    [[nodiscard]] Vector3 getEulerAngles() const
    {
        return m_rotation.getEulerAngles();
    }

    [[nodiscard]] const Vector3 &getScale() const
    {
        return m_scale;
    }

    [[nodiscard]] const Matrix4 &getWorldMatrix() const
    {
        return m_local_to_world;
    }

    [[nodiscard]] const Matrix4 &getLocalMatrix() const
    {
        return m_world_to_local;
    }

    [[nodiscard]] Vector3 getRight() const
    {
        return m_rotation.rotateVector(Vector3::RIGHT).normalize();
    }

    [[nodiscard]] Vector3 getUp() const
    {
        return m_rotation.rotateVector(Vector3::UP).normalize();
    }

    [[nodiscard]] Vector3 getForward() const
    {
        return m_rotation.rotateVector(Vector3::FORWARD).normalize();
    }

    void setPosition(const Vector3 &position)
    {
        m_position = position;
        updateMatrix();
    }

    void setPositionInLocalAxis(const Vector3 &position)
    {
        const auto &pos = position.getAsArray();
        m_position = getForward() * pos[2] + getUp() * pos[1] + getRight() * pos[0];
        updateMatrix();
    }

    void setRotation(const Quaternion &rotation)
    {
        m_rotation = rotation;
        updateMatrix();
    }

    void setEulerAngles(const Vector3 &euler_angles)
    {
        m_rotation = Quaternion::fromEulerAngles(euler_angles);
        updateMatrix();
    }

    void setScale(const Vector3 &scale)
    {
        m_scale = scale;
        updateMatrix();
    }

    Transform &operator*=(const Transform &rhs)
    {
        m_local_to_world *= rhs.m_local_to_world;
        m_world_to_local = m_local_to_world.transformInverse();
        decomposeMatrix();
        return *this;
    }

    Transform operator*(const Transform &rhs) const
    {
        return Transform{m_local_to_world * rhs.m_local_to_world};
    }

    Vector4 operator*(const Vector4 &rhs) const
    {
        return m_local_to_world * rhs;
    }

    void translate(const Vector3 &translation)
    {
        m_position += translation;
        updateMatrix();
    }

    void translateInLocalAxis(const Vector3 &translation)
    {
        const auto &t = translation.getAsArray();
        m_position += getForward() * t[2];
        m_position += getUp() * t[1];
        m_position += getRight() * t[0];
        updateMatrix();
    }

    void rotateX(const float angle)
    {
        m_rotation *= Quaternion::fromAxisAngle(Vector3::RIGHT, angle);
        updateMatrix();
    }

    void rotateY(const float angle)
    {
        m_rotation *= Quaternion::fromAxisAngle(Vector3::UP, angle);
        updateMatrix();
    }

    void rotateZ(const float angle)
    {
        m_rotation *= Quaternion::fromAxisAngle(Vector3::FORWARD, angle);
        updateMatrix();
    }

    void rotate(const Vector3 &euler_angles)
    {
        m_rotation *= Quaternion::fromEulerAngles(euler_angles);
        updateMatrix();
    }

    void rotate(const Quaternion &rotation)
    {
        m_rotation *= rotation;
        updateMatrix();
    }

    void rotateAround(const Vector3 &axis, const float angle)
    {
        m_rotation *= Quaternion::fromAxisAngle(axis, angle);
        updateMatrix();
    }

    [[nodiscard]] Vector4 transformVector(const Vector4 &vec) const
    {
        return *this * vec;
    }

    [[nodiscard]] Vector3 transformPosition(const Vector3 &pos) const
    {
        const auto &arr = pos.getAsArray();
        return static_cast<Vector3>(transformVector(Vector4{arr[0], arr[1], arr[2], 1.0f}));
    }

    [[nodiscard]] Vector3 transformDirection(const Vector3 &dir) const
    {
        return static_cast<Vector3>(transformVector(static_cast<Vector4>(dir)));
    }

  private:
    Vector3 m_position{0.0f};
    Quaternion m_rotation{};
    Vector3 m_scale{1.0f};
    Matrix4 m_local_to_world{};
    Matrix4 m_world_to_local{};

    void updateMatrix()
    {
        m_local_to_world = Matrix4::translation(m_position) * m_rotation.getMatrix() * Matrix4::scale(m_scale);
        m_world_to_local = m_local_to_world.transformInverse();
    }

    void decomposeMatrix()
    {
        const auto decomposed = m_local_to_world.decomposeTransform();
        m_position = static_cast<Vector3>(decomposed[0]);
        m_rotation = Quaternion{decomposed[1]};
        m_scale = static_cast<Vector3>(decomposed[2]);
    }
};

struct HierarchyTransform final : core::IResource
{
    /// Local transform in a scene hierarchy.
    Transform local{};
    /// World transform in a scene hierarchy. (Obtained by chain multiplication of world transforms from root to the
    /// current local transform).
    Transform world{};

    HierarchyTransform() = default;

    explicit HierarchyTransform(const Transform &local, const Transform &world) : local{local}, world{world}
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

#endif // KIRANA_SCENE_TRANSFORM_HPP