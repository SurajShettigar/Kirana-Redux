// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_HANDLE_HPP
#define KIRANA_CORE_HANDLE_HPP

#include <cstdint>
#include <limits>
#include <typeinfo>

namespace kirana::core
{
/**
 * A unique integer-based identifier used for any resource.
 * @tparam T Type of resource.
 */
template <class T>
class Handle
{
    friend class std::hash<Handle>;

public:
    Handle() = default;

    explicit Handle(const uint32_t id): m_id{id}
    {
    }

    ~Handle() = default;

    Handle(const Handle &rhs): m_id{rhs.m_id}
    {
    }

    Handle &operator=(const Handle &rhs)
    {
        m_id = rhs.m_id;
        return *this;
    }

    explicit operator uint32_t() const
    {
        return m_id;
    }

    uint32_t operator()() const
    {
        return m_id;
    }

    bool operator==(const Handle &other) const
    {
        return m_id == other.m_id;
    }

    bool operator!=(const Handle &other) const
    {
        return m_id != other.m_id;
    }

    [[nodiscard]] bool isValid() const
    {
        return m_id < std::numeric_limits<uint32_t>::max();
    }

private:
    uint32_t m_id = std::numeric_limits<uint32_t>::max();
};
}

template <class T> struct std::hash<kirana::core::Handle<T>>
{
    std::uint32_t operator()(const kirana::core::Handle<T> &h) const noexcept
    {
        return h.m_id;
    }
};


#endif //KIRANA_CORE_HANDLE_HPP