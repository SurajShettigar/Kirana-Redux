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
class IResource
{
  public:
    IResource() = default;
    virtual ~IResource() = default;

    [[nodiscard]] virtual bool isValid() const
    {
        return m_status && m_generation != HANDLE_MAX_GENERATION;
    }

    bool load()
    {
        m_status = doLoad() ? 1 : 0;
        m_generation = m_generation == HANDLE_MAX_GENERATION ? 0 : m_generation + 1;
        return m_status;
    }

    void unload()
    {
        doUnload();
        m_status = 0u;
    }

    [[nodiscard]] bool isLoaded() const
    {
        return m_status;
    }

    [[nodiscard]] uint32_t getGeneration() const
    {
        return m_generation;
    }

  protected:
    uint32_t m_generation{HANDLE_MAX_GENERATION};
    uint32_t m_status{0u};

    virtual bool doLoad() = 0;
    virtual void doUnload() = 0;
};

/// Manages resources derived from type IResource. Uses generational indices to keep track of resource lifetime.
/// @tparam T type of the resource.
/// @tparam H type of the resource handle.
template <typename T, typename H> class ResourceManager
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
            m_resources.emplace(m_resources.begin() + index, std::move(resource));
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
        if (!res.load())
        {
            m_free_indices.push(index);
        }

        return Handle<H>(index, res.getGeneration());
    }

    [[nodiscard]] bool isValid(const Handle<H> handle) const
    {
        if (!handle.isValid() || handle.getIndex() >= m_resources.size())
            return false;
        const auto &res = m_resources[handle.getIndex()];
        return res.isLoaded() && res.getGeneration() == handle.getGeneration();
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
        res.unload();
        m_free_indices.push(handle.getIndex());
        return true;
    }

    std::vector<Handle<H>> getAllHandles() const
    {
        std::vector<Handle<H>> result;
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.isLoaded())
            {
                result.emplace_back(i, res.getGeneration());
            }
        }
        return result;
    }

    void forEach(const std::function<void(Handle<H>, T &)> &callback)
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (auto &res = m_resources[i]; res.isLoaded())
            {
                callback(Handle<H>(i, res.getGeneration()), res);
            }
        }
    }

    void forEach(const std::function<void(Handle<H>, const T &)> &callback) const
    {
        for (uint64_t i = 0; i < m_resources.size(); ++i)
        {
            if (const auto &res = m_resources[i]; res.isLoaded())
            {
                callback(Handle<H>(i, res.getGeneration()), res);
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
            if (res.isLoaded())
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
} // namespace kirana::core

#endif // KIRANA_CORE_RESOURCE_MANAGER_HPP