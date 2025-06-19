// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "time_manager.hpp"

#include <chrono>

namespace kirana::core
{
using clock = std::chrono::high_resolution_clock;
constexpr double TICKS_TO_SECONDS =
    clock::duration::period::num /
    static_cast<double>(clock::duration::period::den);
constexpr uint64_t SECONDS_TO_TICKS =
    static_cast<uint64_t>(clock::duration::period::den /
                          static_cast<double>(clock::duration::period::num));

void TimeManager::init()
{
    m_delta_ticks = 0;
    m_prev_ticks = clock::now().time_since_epoch().count();
    m_total_ticks = 0;

    m_frame_count = 0;
    m_fps_count = 0;
    m_fps_ticks = 0;
    m_fps = 0;
}

void TimeManager::tick(const std::function<void()> &callback)
{
    // Get current time
    const auto time = clock::now().time_since_epoch().count();

    // Delta time is current time - time during previous function call.
    m_delta_ticks = time - m_prev_ticks;
    m_prev_ticks = time;

    m_total_ticks += m_delta_ticks;
    m_fps_ticks += m_delta_ticks;

    // Update total frame count since init
    m_frame_count++;
    m_fps_count++;

    // If the current tick is greater than a second, update FPS.
    if (m_fps_ticks >= SECONDS_TO_TICKS)
    {
        m_fps = m_fps_count;
        m_fps_count = 0;
        m_fps_ticks %= SECONDS_TO_TICKS;
    }

    callback();
}

double TimeManager::getDeltaTime() const
{
    return static_cast<double>(m_delta_ticks) * TICKS_TO_SECONDS;
}

double TimeManager::getTotalTime() const
{
    return static_cast<double>(m_total_ticks) * TICKS_TO_SECONDS;
}
}