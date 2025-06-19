// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_MANAGER_HPP
#define KIRANA_CORE_RESOURCE_MANAGER_HPP
#include <unordered_map>

#include "handle.hpp"

namespace kirana::core
{
/**
 * Stores and provide a way to access a vector of uniquely identified resources.
 * @tparam T Resource type.
 */
template <class T>
class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    [[nodiscard]] Handle<T> add(T value)
    {
        size_t index = 0;
        if (m_deleted_indices.empty())
        {
            index = m_values.size();
            m_values.emplace_back(std::move(value));
        }
        else
        {
            index = m_deleted_indices.back();
            m_values[index] = std::move(value);
            m_deleted_indices.pop_back();
        }

        Handle<T> handle(m_counter++);
        m_handles.insert(std::make_pair(handle, index));
        return handle;
    }

    void remove(const Handle<T> handle)
    {
        if (m_handles.find(handle) == m_handles.end())
        {
            return;
        }
        m_deleted_indices.push_back(m_handles.at(handle));
        m_handles.erase(handle);
    }

    [[nodiscard]] size_t get_index(const Handle<T> handle) const
    {
        return m_handles.at(handle);
    }

    [[nodiscard]] const T &get(const Handle<T> handle) const
    {
        return m_values.at(get_index(handle));
    }

    [[nodiscard]] T &get(const Handle<T> handle)
    {
        return m_values.at(get_index(handle));
    }

    [[nodiscard]] const std::vector<T> &get_all() const
    {
        return m_values;
    }

private:
    size_t m_counter = 0;
    std::vector<T> m_values = {};
    std::vector<size_t> m_deleted_indices = {};
    std::unordered_map<Handle<T>, size_t> m_handles = {};
};
}

#endif //KIRANA_CORE_RESOURCE_MANAGER_HPP