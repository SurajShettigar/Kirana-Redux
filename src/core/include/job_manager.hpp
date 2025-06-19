// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_JOB_MANAGER_HPP
#define KIRANA_CORE_JOB_MANAGER_HPP

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include <variant>

#include "no_copy.hpp"
#include "handle.hpp"
#include "resource_manager.hpp"

namespace kirana::core
{
struct DispatchInfo
{
    uint32_t index;
    uint32_t group_index;
};

typedef std::function<void()> Task;
typedef std::function<void(DispatchInfo)> Dispatch;
typedef std::function<void(Handle<Task>)> JobCompleteEventCallback;

struct Job
{
    Handle<Task> handle;
    Task task;
};

/**
 * Thread-safe job queue.
 */
class JobQueue
{
public:
    /// Add the current job to the queue.
    void queue(const Job &job)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        m_jobs.push_front(job);
    }

    /**
     * Get any pending job from the front of the queue.
     * @param job Pointer to the Job object which will be assigned if any pending
     * job is found.
     * @return true, if a pending job was found, else returns false.
     */
    bool dequeue(Job *job)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        if (m_jobs.empty())
            return false;
        *job = std::move(m_jobs.front());
        m_jobs.pop_front();
        return true;
    }

private:
    std::deque<Job> m_jobs;
    std::mutex m_mutex;
};

class JobManager : NoCopy
{
public:
    JobManager() = default;

    ~JobManager()
    {
        clean();
    };

    /**
     * Initializes the job manager. It creates threads based on the CPU core count
     * and other bookkeeping data necessary to handle job scheduling.
     * @return true if initialization was successful.
     */
    bool init();

    /**
     * Adds the given task (function) to a queue and runs them once any thread
     * becomes idle.
     * @param task Function to run.
     * @return Handle to the given task.
     */
    Handle<Task> run(const Task &task) const;

    /**
     * Dispatch is similar to how a compute shader dispatch would work. It is used
     * to run a large batch of similar jobs. Based on the group size, jobs are
     * grouped together with each group running parallel from each other and jobs
     * within a group running sequentially.
     * @param work_size Total number of jobs to run.
     * @param group_size Size of each group. The value should be chosen with care.
     * If the value is too small or too large compared to the work size, the
     * performance will take a hit.
     * @param callback The function which will be called each time a job has been
     * dispatched. You can choose what work to be done inside the callback based
     * on work and group index.
     * @return Vector of handles equal to the number of parallel group dispatches generated.
     */
    std::vector<Handle<Task>> dispatch(uint32_t work_size, uint32_t group_size, const Dispatch &callback) const;

    /// Returns true if any of the job thread is busy.
    bool isBusy() const;
    /// Waits for all the jobs in the queue to be completed.
    void waitForAll() const;

    /**
     * Registers the given callback function which is called when a job is
     * completed.
     * @param callback The function to be called on event.
     */
    void addOnJobCompleteListener(const JobCompleteEventCallback &callback) const
    {
        return m_job_complete_listeners.push_back(callback);
    }

    void clean();

private:
    std::atomic<bool> m_is_running = true;

    uint32_t m_num_threads = 0;
    std::vector<std::thread> m_threads;

    std::mutex m_wake_mutex;
    mutable std::condition_variable m_wake_condition;

    mutable JobQueue m_queue;
    mutable uint32_t m_current_job_count = 0;
    std::atomic<uint32_t> m_completed_job_count = 0;

    mutable std::vector<JobCompleteEventCallback> m_job_complete_listeners;
};
} // namespace kirana::utils

#endif  // KIRANA_CORE_JOB_MANAGER_HPP