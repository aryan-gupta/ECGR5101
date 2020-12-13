#pragma once

#include <opencv2/core/mat.hpp>

#include "ThreadPoolTypes.hpp"
#include "ImageViewer.hpp"

/// A connection handler that only uses one thread
/// (The one that calls `SingleThread::addJob`)
class SingleThread : public ThreadPoolTypes {
public:
    /// Constructs a thread pool using the supplied ImageViewer
    SingleThread(ImageViewer &iv);
    ~SingleThread() = default;


    virtual void stop();
    virtual void addJob(int job);
};