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
    cv::Mat image = receive_image(socket);
    cv::Mat gray = convert_to_grayscale(image);

    send_image(socket, gray);

    {
        // set our flag as finished so we can be removed from
        // the database
        std::lock_guard<std::mutex> lk{ mDatabaseLock };
        mThreads[id].finished.store(true);
    }
}


void MultiThreaded::stop() {
    return;
}

void MultiThreaded::addJob(int job) {
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

    std::cout << "Created thread with id: " << id << std::endl;
}