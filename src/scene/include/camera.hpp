// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_CAMERA_HPP
#define KIRANA_SCENE_CAMERA_HPP

#include "common.hpp"

#include <resource.hpp>

namespace kirana::scene
{
enum class CameraType : uint32_t
{
    PERSPECTIVE = 0u,
    ORTHOGRAPHIC = 1u,
};

class Camera final : public core::IResource
{
  public:
    static constexpr float INFINITE_FAR_CLIPPING_PLANE = 1000000.0f;

    static Camera getPerspective(const std::string &name, const float near_plane, const float far_plane,
                                 const float fov_vertical, const float aspect_ratio)
    {
        return Camera{name, CameraType::PERSPECTIVE, near_plane, far_plane, fov_vertical, aspect_ratio};
    }

    static Camera getOrthographic(const std::string &name, const float near_plane, const float far_plane,
                                  const float size, const float aspect_ratio)
    {
        return Camera{name, CameraType::ORTHOGRAPHIC, near_plane, far_plane, size, aspect_ratio};
    }


    static Camera getOrthographicFromBox(const std::string &name, const float near_plane, const float far_plane,
                                         const float left, const float right, const float bottom, const float top)
    {
        return Camera{name,      CameraType::ORTHOGRAPHIC, near_plane,
                      far_plane, (right - left) * 0.5f,    std::fabs((right - left) / (top - bottom))};
    }

    static Camera getOrthographicFromSize2D(const std::string &name, const float near_plane, const float far_plane,
                                            const float size_x, const float size_y)
    {
        return Camera{name, CameraType::ORTHOGRAPHIC, near_plane, far_plane, size_x, size_x / size_y};
    }

    Camera() : IResource{"Camera"}
    {
    }
    explicit Camera(const std::string &name) : IResource{name}
    {
    }
    explicit Camera(const std::string &name, const CameraType type, const float near_plane, const float far_plane,
                    const float fov_vertical_size, const float aspect_ratio)
        : IResource{name}, m_type{type}, m_near_plane{near_plane}, m_far_plane{far_plane},
          m_fov_y_size{fov_vertical_size}, m_aspect_ratio{aspect_ratio}
    {
    }

    [[nodiscard]] CameraType getType() const
    {
        return m_type;
    }

    [[nodiscard]] float getClippingPlaneNear() const
    {
        return m_near_plane;
    }

    [[nodiscard]] float getClippingPlaneFar() const
    {
        return m_far_plane;
    }

    void setClippingPlanes(const std::array<float, 2> &planes)
    {
        m_near_plane = planes[0];
        m_far_plane = planes[1];
        m_is_dirty = true;
    }

    [[nodiscard]] std::optional<float> getPerspectiveFieldOfView() const
    {
        if (m_type == CameraType::PERSPECTIVE)
        {
            return m_fov_y_size;
        }
        return std::nullopt;
    }

    bool setPerspectiveFieldOfView(const float fov_vertical)
    {
        if (m_type == CameraType::PERSPECTIVE)
        {
            m_fov_y_size = fov_vertical;
            m_is_dirty = true;
            return true;
        }
        return false;
    }

    [[nodiscard]] std::optional<float> getOrthographicSize() const
    {
        if (m_type == CameraType::ORTHOGRAPHIC)
        {
            return m_fov_y_size;
        }
        return std::nullopt;
    }

    bool setOrthographicSize(const float size)
    {
        if (m_type == CameraType::ORTHOGRAPHIC)
        {
            m_fov_y_size = size;
            m_is_dirty = true;
            return true;
        }
        return false;
    }

    bool setOrthographicSize2D(const float size_x, const float size_y)
    {
        if (m_type == CameraType::ORTHOGRAPHIC)
        {
            m_aspect_ratio = size_x / size_y;
            m_fov_y_size = size_x;
            m_is_dirty = true;
            return true;
        }
        return false;
    }

    [[nodiscard]] float getAspectRatio() const
    {
        return m_aspect_ratio;
    }

    void setAspectRatio(const float aspect_ratio)
    {
        m_aspect_ratio = aspect_ratio;
        m_is_dirty = true;
    }

    [[nodiscard]] const Matrix4 &getProjectionMatrix() const
    {
        if (m_is_dirty)
        {
            updateProjectionMatrix();
        }
        return m_projection_matrix;
    }

  protected:
    CameraType m_type{CameraType::PERSPECTIVE};

    float m_near_plane{0.0001f};
    float m_far_plane{1000.0f};
    // Value will store the vertical field of view in degree for perspective camera or the size of the box in case of
    // orthographic camera.
    float m_fov_y_size{50.0f};
    float m_aspect_ratio{1.0f};

    mutable bool m_is_dirty{true};
    mutable Matrix4 m_projection_matrix{};

    void updateProjectionMatrix() const
    {
        switch (m_type)
        {
        case CameraType::PERSPECTIVE: {
            m_projection_matrix =
                Matrix4::perspectiveProjectionRH(m_fov_y_size, m_aspect_ratio, m_near_plane, m_far_plane);
        }
        break;
        case CameraType::ORTHOGRAPHIC: {
            m_projection_matrix =
                Matrix4::orthographicProjectionRH(m_fov_y_size, m_aspect_ratio, m_near_plane, m_far_plane);
        }
        break;
        }
        m_is_dirty = false;
    }

    bool doLoad() override
    {
        return true;
    }

    void doUnload() override
    {
    }
};
} // namespace kirana::scene

#endif // KIRANA_SCENE_CAMERA_HPP