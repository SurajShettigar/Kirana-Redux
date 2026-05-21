// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_HANDLE_HPP
#define KIRANA_CORE_HANDLE_HPP

#include <cstdint>
#include <limits>

namespace kirana::core
{
/// An integer-based unique identifier associated with a resource.
template <class T> class Handle final
{
    friend class std::hash<Handle>;

  public:
    Handle() : m_value{std::numeric_limits<uint64_t>::max()}
    {
    }

    ~Handle() = default;

    Handle(const Handle &other) = default;
    Handle &operator=(const Handle &other) = default;

    explicit Handle(const uint64_t value) : m_value{value}
    {
    }

    explicit Handle(const uint64_t index, const uint16_t generation)
        : m_index{index}, m_generation{generation}
    {
    }

    [[nodiscard]] bool isValid() const
    {
        return m_value != std::numeric_limits<uint64_t>::max();
    }

    [[nodiscard]] uint64_t getIndex() const
    {
        return m_index;
    }

    [[nodiscard]] uint16_t getGeneration() const
    {
        return m_generation;
    }

    explicit operator uint64_t() const
    {
        return m_value;
    }

    bool operator==(const Handle &other) const
    {
        return m_value == other.m_value;
    }

    bool operator!=(const Handle &other) const
    {
        return m_value != other.m_value;
    }

    explicit operator bool() const
    {
        return isValid();
    }
  private:
    union {
        struct
        {
            uint64_t m_index : 48;
            uint64_t m_generation : 16;
        };

        uint64_t m_value;
    };
};
} // namespace kirana::core

template <class T> struct std::hash<kirana::core::Handle<T>>
{
    std::size_t operator()(const kirana::core::Handle<T> &h) const noexcept
    {
        return h.m_value;
    }
};

#endif // KIRANA_CORE_HANDLE_HPP
