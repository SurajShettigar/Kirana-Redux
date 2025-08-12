// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_HANDLE_HPP
#define KIRANA_CORE_HANDLE_HPP

#include <cstdint>
#include <bit>

namespace kirana::core
{
static constexpr uint64_t HANDLE_INDEX_BITS = 48ull;
static constexpr uint64_t HANDLE_MAX_INDEX = (1ull << HANDLE_INDEX_BITS) - 1ull;
static constexpr uint16_t HANDLE_GENERATION_BITS = 16u;
static constexpr uint16_t HANDLE_MAX_GENERATION = (1u << HANDLE_GENERATION_BITS) - 1u;
static constexpr uint64_t HANDLE_MAX_VALUE = static_cast<uint64_t>(HANDLE_MAX_GENERATION) << HANDLE_INDEX_BITS |
                                             HANDLE_MAX_INDEX;

/// An integer-based unique identifier associated with a resource.
template <class T>
class Handle final
{
    friend class std::hash<Handle>;

public:
    Handle(): m_value{HANDLE_MAX_VALUE}
    {
    }

    virtual ~Handle() = default;

    explicit Handle(const uint64_t value): m_value{value}
    {
    }

    explicit Handle(const uint64_t index, const uint16_t generation): m_index{index}, m_generation{generation}
    {
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

    [[nodiscard]] bool isValid() const
    {
        return m_index < HANDLE_MAX_INDEX && m_generation < HANDLE_MAX_GENERATION;
    }

    [[nodiscard]] uint64_t getIndex() const
    {
        return m_index;
    }

    [[nodiscard]] uint16_t getGeneration() const
    {
        return m_generation;
    }

protected:
    union
    {
        struct
        {
            uint64_t m_index : 48;
            uint64_t m_generation : 16;
        };

        uint64_t m_value;
    };
};
}

template <class T>
struct std::hash<kirana::core::Handle<T>>
{
    std::size_t operator()(const kirana::core::Handle<T> &h) const noexcept
    {
        return h.m_value;
    }
};


#endif //KIRANA_CORE_HANDLE_HPP