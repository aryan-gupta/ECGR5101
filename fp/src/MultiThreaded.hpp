#pragma once

#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
#include <memory>

#include "ThreadPoolTypes.hpp"

/// A connection manager that starts a thread for every
/// connection. The thread is started by the thread calling
/// `MultiThreaded::addJob`
class MultiThreaded : public ThreadPoolTypes {
    /// The last thread ID so each  thread can have its own ID
    static int mIDStart;

    /// Stores the data one one thread. Helps with removing finished
    /// threads
    struct ThreadData {
        std::atomic_bool finished;
        std::thread thread;
    };

    /// A lock protecting the database of threads
    std::mutex mDatabaseLock;
    std::unordered_map<int, ThreadData> mThreads;

    /// The thread target, this is what each thread runs
    void thread_target(int id, int socket);

public:
    /// Constructs a Multithreaded connection manager using the
    /// supplied ImageViewer
    MultiThreaded();

    virtual void stop();
    virtual void addJob(int job);
};