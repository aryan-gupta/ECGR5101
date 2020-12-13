#include "ThreadPool.hpp"

#include "main.hpp"
#include "image_converter.hpp"
#include "network.hpp"

ThreadPool::ThreadPool(ImageViewer &iv, size_t numThreads)
    : ThreadPoolTypes{ iv } {
    // create numThreads threads
    while (numThreads --> 0) {
        mThreads.emplace_back(&ThreadPool::thread_target, this);
    }
}

void ThreadPool::stop() {
    return;
}

void ThreadPool::addJob(int job) {
    {
        // add the job after we've accurired the lock
        std::unique_lock<std::mutex> lk{ mJobLock };
        mJobs.push(job);
    }

    // notify one thread that more data is available
    mJobSignal.notify_one();
}

void ThreadPool::thread_target() {
    while (true) {
        int job = 0;
        
        {
            // attempt to pull a data off of the queue
            // if we cant, then wait on the condition var
            std::unique_lock<std::mutex> lk{ mJobLock };
            mJobSignal.wait(lk);

            // if we were waken up but the queue is still empty
            // then loop and continue waiting.
            // otherwise remove a job from the queue
            if (mJobs.empty()) {
                continue;
            } else {
                job = mJobs.front();
                mJobs.pop();
            }
        }

        // do the job
        ImageViewer::FinishedJob fd{  };
        fd.image = receive_image(job);
        fd.gray = convert_to_grayscale(fd.image);
        
        // signal the image viewer thread
        mViewer.signal(fd);
    }
}