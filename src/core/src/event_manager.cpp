// Copyright 2026 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "event_manager.hpp"

namespace kirana::core
{
void EventManager::dispatchEvent(const Event &event) const
{
    if (m_dispatch_mode == EventDispatchMode::IMMEDIATE)
    {
        for (const auto &listener : m_listeners)
        {
            listener->onEvent(event);
        }
    }
    else
    {
        std::scoped_lock lock(m_queue_mutex);
        m_queue_events.push(std::unique_ptr<Event>(event.clone()));
    }
}

void EventManager::pollEvents() const
{
    if (m_dispatch_mode == EventDispatchMode::IMMEDIATE || m_queue_events.empty())
    {
        return;
    }
    std::queue<std::unique_ptr<Event>> events;
    {
        std::scoped_lock lock(m_queue_mutex);
        std::swap(events, m_queue_events);
    }
    while (!events.empty())
    {
        const auto &event = *events.front();
        for (const auto &listener : m_listeners)
        {
            listener->onEvent(event);
        }
        events.pop();
    }
}
}