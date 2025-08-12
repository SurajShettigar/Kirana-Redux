// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_MANAGER_HPP
#define KIRANA_CORE_RESOURCE_MANAGER_HPP

#include "handle.hpp"

#include <vector>
#include <queue>
#include <optional>

namespace kirana::core
{
/// Resource interface with generation and alive tracking
struct IResource
{
    uint16_t generation{HANDLE_MAX_GENERATION};
    bool alive{false};

    virtual ~IResource() = default;
};

/// Manages resources derived from type IResource. Uses generational indices to keep track of resource lifetime.
/// @tparam T type of the resource.
/// @tparam H type of the resource handle.
template <typename T, typename H>
class ResourceManager
{
    static_assert(std::is_base_of_v<IResource, T>, "T must derive from IResource");

public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    [[nodiscard]] Handle<H> add(T &&resource)
    {
        uint64_t index;

        if (!m_free_indices.empty())
        {
            index = m_free_indices.front();
            m_free_indices.pop();
            m_resources[index] = std::move(resource);
        }
        else
        {
            index = m_resources.size();
            if (index > HANDLE_MAX_INDEX)
            {
                return Handle<H>();
            }
            m_resources.emplace_back(std::move(resource));
        }

        auto &res = m_resources[index];
        res.alive = true;
        res.generation = res.generation == HANDLE_MAX_GENERATION ? 0 : res.generation + 1;

        return Handle<H>(index, res.generation);
    }

    [[nodiscard]] bool isValid(const Handle<H> handle) const
    {
        if (!handle.isValid() || handle.getIndex() >= m_resources.size())
            return false;
        const auto &res = m_resources[handle.getIndex()];
        return res.alive && res.generation == handle.generation;
    }

    std::optional<T &> get(const Handle<H> handle)
    {
        if (!isValid(handle))
            return std::nullopt;
        return &m_resources[handle.index];
    }

    std::optional<const T &> get(const Handle<H> handle) const
    {
        if (!isValid(handle))
            return std::nullopt;
        return &m_resources[handle.index];
    }

    bool remove(const Handle<H> handle)
    {
        if (!isValid(handle))
            return false;

        auto &res = m_resources[handle.index];
        res = T();
        res.alive = false;
        m_free_indices.push(handle.index);
        return true;
    }

    std::vector<Handle<H>> getAllHandles() const
    {
        std::vector<Handle<H>> result;
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.alive)
            {
                result.emplace_back(i, res.generation);
            }
        }
        return result;
    }

    template <typename Func>
    void forEach(Func &&func)
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (auto &res = m_resources[i]; res.alive)
            {
                func(HandleType(i, res.generation), res);
            }
        }
    }

    template <typename Func>
    void forEach(Func &&func) const
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.alive)
            {
                func(HandleType(i, res.generation), res);
            }
        }
    }

    std::vector<T> &getResources()
    {
        return m_resources;
    }

    const std::vector<T> &getResources() const
    {
        return m_resources;
    }

    size_t getSize() const
    {
        size_t count = 0;
        for (const auto &res : m_resources)
        {
            if (res.alive)
                count++;
        }
        return count;
    }

    size_t getFreeSlotCount() const
    {
        return m_free_indices.size();
    }

    void clear()
    {
        m_resources.clear();
        while (!m_free_indices.empty())
        {
            m_free_indices.pop();
        }
    }

private:
    std::vector<T> m_resources;
    std::queue<uint64_t> m_free_indices;
};
}

#endif //KIRANA_CORE_RESOURCE_MANAGER_HPP