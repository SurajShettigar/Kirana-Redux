// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_MANAGER_HPP
#define KIRANA_CORE_RESOURCE_MANAGER_HPP

#include "handle.hpp"

#include <functional>
#include <queue>
#include <vector>

namespace kirana::core
{
/// Resource interface with generation and alive tracking
struct IResource
{
    uint32_t generation : 16 {HANDLE_MAX_GENERATION};
    uint32_t status : 16 {0u};

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

    [[nodiscard]] Handle<H> add(const T &resource)
    {
        uint64_t index;

        if (!m_free_indices.empty())
        {
            index = m_free_indices.front();
            m_free_indices.pop();
            m_resources[index] = resource;
        }
        else
        {
            index = m_resources.size();
            if (index > HANDLE_MAX_INDEX)
            {
                return Handle<H>();
            }
            m_resources.push_back(resource);
        }

        auto &res = m_resources[index];
        res.status = 1u;
        res.generation = res.generation == HANDLE_MAX_GENERATION ? 0 : res.generation + 1;

        return Handle<H>(index, res.generation);
    }

    [[nodiscard]] bool isValid(const Handle<H> handle) const
    {
        if (!handle.isValid() || handle.getIndex() >= m_resources.size())
            return false;
        const auto &res = m_resources[handle.getIndex()];
        return res.status && res.generation == handle.getGeneration();
    }

    T *get(const Handle<H> handle)
    {
        if (!isValid(handle))
            return nullptr;
        return &m_resources[handle.getIndex()];
    }

    const T *get(const Handle<H> handle) const
    {
        if (!isValid(handle))
            return nullptr;
        return &m_resources[handle.getIndex()];
    }

    bool remove(const Handle<H> handle)
    {
        if (!isValid(handle))
            return false;

        auto &res = m_resources[handle.getIndex()];
        res = T();
        res.status = 0u;
        m_free_indices.push(handle.getIndex());
        return true;
    }

    std::vector<Handle<H>> getAllHandles() const
    {
        std::vector<Handle<H>> result;
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.status)
            {
                result.emplace_back(i, res.generation);
            }
        }
        return result;
    }

    void forEach(const std::function<void(Handle<H>, T &)> &callback)
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (auto &res = m_resources[i]; res.status)
            {
                callback(Handle<H>(i, res.generation), res);
            }
        }
    }

    void forEach(const std::function<void(Handle<H>, const T &)> &callback) const
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.status)
            {
                callback(Handle<H>(i, res.generation), res);
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

    [[nodiscard]] size_t getSize() const
    {
        size_t count = 0;
        for (const auto &res : m_resources)
        {
            if (res.status)
                count++;
        }
        return count;
    }

    [[nodiscard]] size_t getFreeSlotCount() const
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