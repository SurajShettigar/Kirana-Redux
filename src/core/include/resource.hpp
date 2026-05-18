// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_RESOURCE_HPP
#define KIRANA_CORE_RESOURCE_HPP

#include <string>

namespace kirana::core
{
/// Resource interface with generation and alive tracking
class IResource
{
    template <typename T> friend class ResourceManager;

  public:
    explicit IResource(std::string name) : m_name{std::move(name)}
    {
    }

    virtual ~IResource() = default;

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = std::move(name);
    }

    [[nodiscard]] virtual bool isValid() const
    {
        return m_status;
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
    uint16_t m_generation{std::numeric_limits<uint16_t>::max()};
    uint16_t m_status{0u};
    uint16_t m_is_loaded{0u};

    std::string m_name{};

    virtual bool doLoad() = 0;
    virtual void doUnload() = 0;
};
} // namespace kirana::core
#endif // KIRANA_CORE_RESOURCE_HPP
