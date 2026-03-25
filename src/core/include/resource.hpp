// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_HPP
#define KIRANA_CORE_RESOURCE_HPP

#include "handle.hpp"

namespace kirana::core
{
/// Resource interface with generation and alive tracking
class IResource
{
    template <typename T, typename H> friend class ResourceManager;

  public:
    IResource() = default;
    virtual ~IResource() = default;

    [[nodiscard]] virtual bool isValid() const
    {
        return m_status && m_generation != HANDLE_MAX_GENERATION;
    }

    bool load()
    {
        m_is_loaded = doLoad() ? 1u : 0u;
        return m_is_loaded;
    }

    void unload()
    {
        doUnload();
        m_is_loaded = 0u;
    }

    [[nodiscard]] bool isLoaded() const
    {
        return m_is_loaded;
    }

  protected:
    uint32_t m_generation{HANDLE_MAX_GENERATION};
    uint16_t m_status{0u};
    uint16_t m_is_loaded{0u};

    virtual bool doLoad() = 0;
    virtual void doUnload() = 0;
};
} // namespace kirana::core
#endif // KIRANA_CORE_RESOURCE_HPP
