// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_EVENT_MANAGER_HPP
#define KIRANA_CORE_EVENT_MANAGER_HPP

#include <string>
#include <vector>
#include <ranges>
#include <queue>
#include <mutex>

#include "no_copy.hpp"

namespace kirana::core
{
struct Event
{
    Event() = default;
    virtual ~Event() = default;

    [[nodiscard]] virtual const char *getType() const = 0;

    [[nodiscard]] virtual Event *clone() const = 0;
};

#define DEFINE_EVENT_TYPE(type)                                                                                        \
    static const char *getStaticType()                                                                                 \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }                                                                                                                  \
    const char *getType() const override                                                                       \
    {                                                                                                                  \
        return getStaticType();                                                                                        \
    }                                                                                                                  \
    Event *clone() const override                                                                              \
    {                                                                                                                  \
        return new type(*this);                                                                                        \
    }

class EventListener
{
  public:
    EventListener() = default;
    virtual ~EventListener() = default;
    virtual void onEvent(const Event &event) = 0;
};

enum class EventDispatchMode
{
    IMMEDIATE = 0,
    QUEUED = 1,
};

class EventManager : NoCopy
{
  public:
    EventManager() = default;
    explicit EventManager(const EventDispatchMode dispatch_mode) : m_dispatch_mode{dispatch_mode}
    {
    }

    [[nodiscard]] EventDispatchMode getDispatchMode() const
    {
        return m_dispatch_mode;
    }

    void setDispatchMode(const EventDispatchMode dispatch_mode)
    {
        m_dispatch_mode = dispatch_mode;
    }

    void addListener(EventListener *listener)
    {
        m_listeners.push_back(listener);
    }

    void removeListener(const EventListener *listener)
    {
        if (const auto it = std::ranges::find(m_listeners, listener); it != m_listeners.end())
        {
            m_listeners.erase(it);
        }
    }

    void dispatchEvent(const Event &event) const;

    void pollEvents() const;
  private:
    EventDispatchMode m_dispatch_mode = EventDispatchMode::IMMEDIATE;
    std::vector<EventListener *> m_listeners{};
    mutable std::queue<std::unique_ptr<Event>> m_queue_events{};
    mutable std::mutex m_queue_mutex{};
};
} // namespace kirana::core

#endif // KIRANA_CORE_EVENT_MANAGER_HPP
