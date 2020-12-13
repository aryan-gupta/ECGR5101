#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

#include "ThreadPoolTypes.hpp"

/// Creates a thread pool to manage each connection
class ThreadPool : public ThreadPoolTypes {
protected:
    /// An array of all the threads
    std::vector<std::thread> mThreads;

    /// A mutex to lock the queue and prevent race conditions
    std::mutex mJobLock;
    /// A condition variable to signal that more jobs are available
    std::condition_variable mJobSignal;
    /// A Queue to hold the jobs
    std::queue<int> mJobs;

    /// A flag to signal the threads to stop so we can gracefully exit
    std::atomic_flag mStop;

    // The thread target function. This is what each thread runs
    void thread_target(int id);

public:
    /// Constructs a thread pool using the supplied ImageViewer and
    /// number of threads
    ThreadPool(size_t numThreads = 5);
    ~ThreadPool() = default;


    virtual void stop();
    virtual void addJob(int job);
};