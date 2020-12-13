#pragma once

#include "ThreadPool.hpp"

/// A simple class that changes the ThreadPool class to
/// have a higher or lower priority
/// @todo this can be templated so the priority can be applied
///       to any of the connection managers
/// @note there are many items here that are dependent on
///       the fact that you are runing a unix OS and a pthread
///       backend. Do not expect for this to work on Windows
class ThreadPoolPriority : public ThreadPool {
public:
    /// Creates the connection manager using the ImageViewer provided
    /// and the number of threads. Also sets the priority of the thread
    ThreadPoolPriority(size_t numThreads = 5);
};