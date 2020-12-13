#pragma once

#include "ImageViewer.hpp"

/// The base class for all connection managers
class ThreadPoolTypes {
protected:
    /// Stores the image viewer so there is no deadlock
    /// Only one thread can show the images or a deadlock
    /// will occur
    ImageViewer &mViewer;

public:
    /// Creates the class using the ImageViewer provided
    ThreadPoolTypes(ImageViewer &iv);

    /// A pure virtual function that stops the 
    /// connection manager
    virtual void stop() = 0;

    /// A pure virtual function that queues up a job
    /// for the connection manager to handle
    virtual void addJob(int job) = 0;
};