#include "MultiThreaded.hpp"

#include <iostream>
#include <utility>

#include "main.hpp"
#include "image_converter.hpp"
#include "network.hpp"

int MultiThreaded::mIDStart = 1;

MultiThreaded::MultiThreaded()
    : ThreadPoolTypes{  } {  }

void MultiThreaded::thread_target(int id, int socket) {
    std::cout << "MultiThreaded: Thread " << id << ": Started" << std::endl;

    std::cout << "MultiThreaded: Thread " << id << " Image processing started" << std::endl;
    ThreadPoolTypes::process_job(socket);
    std::cout << "MultiThreaded: Thread " << id << " Image processed" << std::endl;

    {
        // set our flag as finished so we can be removed from
        // the database
        std::lock_guard<std::mutex> lk{ mDatabaseLock };
        mThreads[id].finished.store(true);
    }

    std::cout << "MultiThreaded: Thread " << id << " Finished, exiting" << std::endl;
}


void MultiThreaded::stop() {
    return;
}

void MultiThreaded::addJob(int job) {
       std::cout << "MultiThreaded: Creating Thread for job" << std::endl;

    // get the next id for the thread
    int id = mIDStart++;

    ThreadData *td = nullptr;
    
    {
        // add a new entry to the database
        std::lock_guard<std::mutex> lk{ mDatabaseLock };
        td = &mThreads[id];
    }

    // construct the thread
    td->finished.store(false);
    td->thread = std::thread{ &MultiThreaded::thread_target, this, id, job };

    std::cout << "MultiThreaded: Created thread with id: " << id << std::endl;
}