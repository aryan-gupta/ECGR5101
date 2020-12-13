#pragma once

#include <thread>
#include <atomic>

#include "ThreadPoolTypes.hpp"

/// The main connection server. Manages the socket
/// and gives the connection manager work to do
class MainServer {
    /// The server thread. Since the server is a separate
    /// thread, the stdin is available for input from the
    /// main thread
    std::thread mThread;
    /// The connection manager that manages the threads
    ThreadPoolTypes *mPool;
    /// A flag variable signaling to stop the server
    std::atomic_bool mStop;
    
    /// The target of the server's thread. This is what
    /// mThread runs
    void thread_target() const;

public:
    /// Create a socket server using the pool supplied
    MainServer(ThreadPoolTypes *const pool);

    /// Runs the server. Will change the priority if the
    // priority parameter is set to true
    void run(bool priority);

    /// Stops the server
    void stop();
};