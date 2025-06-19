// Copyright 2024 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_TIME_MANAGER_HPP
#define KIRANA_CORE_TIME_MANAGER_HPP

#include <cstdint>
#include <functional>

#include "no_copy.hpp"

namespace kirana::core
{
/**
 * Provides time related functionality to the application.
 */
class TimeManager : NoCopy
{
public:
    TimeManager() = default;
    ~TimeManager() = default;

    /**
     * Initializes the clock.
     */
    void init();

    /**
     * Updates clock tick and runs the callback function on tick. If the clock is
     * initialized with a fixed time-step, the callback is called after the fixed
     * time-step.
     * @param callback Function to be called on clock tick.
     */
    void tick(const std::function<void()> &callback);

    /**
     * Time interval in seconds from last tick to current tick call.
     * @return Double precision floating point number representing delta time.
     */
    [[nodiscard]] double getDeltaTime() const;

    /**
     * Time that has been elapsed in seconds since timer initialization.
     * @return Double precision floating point number representing total time
     * since initialization.
     */
    [[nodiscard]] double getTotalTime() const;

    /**
     * Number of frames per second.
     * @return Unsigned 64-bit integer representing frames per second.
     */
    [[nodiscard]] uint64_t getFPS() const
    {
        return m_fps;
    }

    /**
     * Get number of frames since timer initialization.
     * @return Unsigned 64-bit integer representing total frames since
     * initialization.
     */
    [[nodiscard]] uint64_t getTotalFrameCount() const
    {
        return m_frame_count;
    }

private:
    uint64_t m_prev_ticks = 0;
    uint64_t m_delta_ticks = 0;
    uint64_t m_total_ticks = 0;

    uint64_t m_frame_count = 0;
    uint64_t m_fps_count = 0;
    uint64_t m_fps_ticks = 0;
    uint64_t m_fps = 0;
};
}

#endif  // KIRANA_CORE_TIME_MANAGER_HPP