// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_MANAGER_HPP
#define KIRANA_CORE_RESOURCE_MANAGER_HPP

#include "resource.hpp"
#include "handle.hpp"

#include <functional>
#include <queue>
#include <vector>

namespace kirana::core
{

/// Manages resources derived from type IResource. Uses generational indices to keep track of resource lifetime.
/// @tparam T type of the resource.
template <typename T> class ResourceManager
{
    static_assert(std::is_base_of_v<IResource, T>, "T must derive from IResource");

  public:
    ResourceManager() = default;
    explicit ResourceManager(const bool auto_load_resource) : m_auto_load_resource{auto_load_resource}
    {
    }
    ~ResourceManager() = default;

    template <typename... Args> [[nodiscard]] Handle<T> add(Args &&...args)
    {
        uint64_t index;

        if (!m_free_indices.empty())
        {
            index = m_free_indices.front();
            m_free_indices.pop();
            m_resources.emplace(m_resources.begin() + index, std::forward<Args>(args)...);
        }
        else
        {
            index = m_resources.size();
            m_resources.emplace_back(std::forward<Args>(args)...);
        }

        auto &res = m_resources[index];
        res.m_status = 1u;
        res.m_generation = res.m_generation == std::numeric_limits<uint16_t>::max() ? 0 : res.m_generation + 1;
        if (m_auto_load_resource)
        {
            res.load();
        }
        return Handle<T>(index, res.m_generation);
    }

    [[nodiscard]] bool isValid(const Handle<T> handle) const
    {
        const auto index = handle.getIndex();
        if (index >= m_resources.size())
        {
            return false;
        }
        const auto &res = m_resources[index];
        return res.m_status && res.m_generation == handle.getGeneration();
    }

    T *get(const Handle<T> handle)
    {
        if (!isValid(handle))
            return nullptr;
        return &m_resources[handle.getIndex()];
    }

    const T *get(const Handle<T> handle) const
    {
        if (!isValid(handle))
            return nullptr;
        return &m_resources[handle.getIndex()];
    }

    /// Removes (and unloads) resource with the given handle and sets it's state to invalid. It's memory will be reused
    /// by another resource added in the future.
    bool remove(const Handle<T> handle)
    {
        if (!isValid(handle))
            return false;

        doRemove(handle.getIndex());
        return true;
    }

    /// Removes (and unloads) all resources and sets it's state to invalid. It's memory will be reused by other
    /// resources added in the future.
    void removeAll()
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.m_status)
            {
                doRemove(i);
            }
        }
    }

    [[nodiscard]] size_t getSize() const
    {
        size_t count = 0;
        for (const auto &res : m_resources)
        {
            if (res.m_status)
                count++;
        }
        return count;
    }

    std::vector<Handle<T>> getAllHandles() const
    {
        std::vector<Handle<T>> result;
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.m_status)
            {
                result.emplace_back(i, res.m_generation);
            }
        }
        return result;
    }

    void forEach(const std::function<void(Handle<T>, T &)> &callback)
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (auto &res = m_resources[i]; res.m_status)
            {
                callback(Handle<T>(i, res.m_generation), res);
            }
        }
    }

    void forEach(const std::function<void(Handle<T>, const T &)> &callback) const
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.m_status)
            {
                callback(Handle<T>(i, res.m_generation), res);
            }
        }
    }

    Handle<T> findIf(const std::function<bool(const T &)> &callback) const
    {
        const auto it = std::ranges::find_if(m_resources, callback);
        if (it == m_resources.end())
        {
            return {};
        }
        return Handle<T>(std::distance(m_resources.begin(), it), it->m_generation);
    }

    /// Removes all resources and clears the memory.
    void clear()
    {
        removeAll();
        m_resources.clear();
        while (!m_free_indices.empty())
        {
            m_free_indices.pop();
        }
    }

  private:
    bool m_auto_load_resource{true};
    std::vector<T> m_resources;
    std::queue<uint64_t> m_free_indices;

    void doRemove(const uint64_t index)
    {
        auto &res = m_resources[index];
        res.unload();
        res.m_status = 0u;
        m_free_indices.push(index);
    }
};
} // namespace kirana::core
#endif // KIRANA_CORE_RESOURCE_MANAGER_HPP