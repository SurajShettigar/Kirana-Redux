// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "job_manager.hpp"

#include <algorithm>

namespace kirana::core
{
bool JobManager::init()
{
    m_is_running.store(true);

    // Set number of threads to number of physical and virtual CPU cores
    // available.
    m_num_threads = std::max(1u, std::thread::hardware_concurrency());
    m_threads.clear();
    m_threads.reserve(m_num_threads);

    m_current_job_count = 0;
    m_completed_job_count.store(0);

    // Initialize all the threads. Threads run indefinitely until we explicitly
    // stop it or if JobManager object has been destroyed. It checks for jobs in
    // the queue and executes it. If no job is found, it goes back to sleep until
    // notified of a new job.
    for (size_t i = 0; i < m_num_threads; i++)
    {
        m_threads.emplace_back([&]() {
            while (m_is_running.load())
            {
                Job curr_job;
                if (m_queue.dequeue(&curr_job))
                {
                    // Execute the current task and increment the completed job count.
                    curr_job.task();
                    m_completed_job_count.fetch_add(1);
                    // Send job completed event.
                    for (const auto &l : m_job_complete_listeners)
                        l(curr_job.handle);
                }
                else
                {
                    // The condition variable ensures that the current thread goes to
                    // sleep until notify_one() or notify_all() is called.
                    std::unique_lock<std::mutex> lock(m_wake_mutex);
                    m_wake_condition.wait(lock);
                }
            }
        });
    }

    return true;
}

Handle<Task> JobManager::run(const Task &task) const
{
    const auto handle = Handle<Task>(m_current_job_count);
    const auto job = Job{handle, task};

    m_current_job_count += 1;
    m_queue.queue(job);
    m_wake_condition.notify_one();
    return handle;
}

std::vector<Handle<Task>> JobManager::dispatch(const uint32_t work_size, const uint32_t group_size,
                                               const Dispatch &callback) const
{
    if (work_size == 0 || group_size == 0)
        return {};

    const uint32_t group_count = (work_size + group_size - 1) / group_size;

    std::vector<Handle<Task>> handles = {};
    handles.reserve(group_count);
    for (uint32_t i = 0; i < group_count; i++)
    {
        const uint32_t w_start = i * group_size;
        const uint32_t w_end = std::min(w_start + group_size, work_size);

        const auto handle = Handle<Task>(m_current_job_count);
        auto job = Job{handle};
        // Each job runs a group of user-defined tasks sequentially.
        job.task = [w_start, w_end, i, callback]() {
            for (uint32_t w = w_start; w < w_end; w++)
            {
                callback(DispatchInfo{w, i});
            }
        };
        m_current_job_count += 1;
        m_queue.queue(job);
        m_wake_condition.notify_one();
        handles.push_back(handle);
    }
    return handles;
}

bool JobManager::isBusy() const
{
    return m_completed_job_count.load() < m_current_job_count;
}

void JobManager::waitForAll() const
{
    while (isBusy())
    {
        m_wake_condition.notify_one();
        std::this_thread::yield();
    }
}

void JobManager::clean()
{
    if (m_threads.empty())
        return;
    waitForAll();
    m_is_running.store(false);
    m_wake_condition.notify_all();
    for (auto &t : m_threads)
        t.join();
    m_threads.clear();
    m_current_job_count = 0;
    m_completed_job_count = 0;
}
} // namespace kirana::utils