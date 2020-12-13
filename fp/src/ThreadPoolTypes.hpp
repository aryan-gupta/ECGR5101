#pragma once


/// The base class for all connection managers
class ThreadPoolTypes {
protected:
    static void process_job(int socket);

public:
    /// Creates the class using the ImageViewer provided
    ThreadPoolTypes() = default;
    virtual ~ThreadPoolTypes() = default;

    /// A pure virtual function that stops the 
    /// connection manager
    virtual void stop() = 0;

    /// A pure virtual function that queues up a job
    /// for the connection manager to handle
    virtual void addJob(int job) = 0;
};