#include "ThreadPool.hpp"

#include <iostream>

#include "main.hpp"
#include "image_converter.hpp"
#include "network.hpp"

ThreadPool::ThreadPool(size_t numThreads)
    : ThreadPoolTypes{  } {
    // create numThreads threads
    while (numThreads --> 0) {
        mThreads.emplace_back(&ThreadPool::thread_target, this, numThreads);
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

void ThreadPool::thread_target(int id) {
    std::cout << "ThreadPool: Thread " << id << ": Started" << std::endl;
    while (true) {
        int job = 0;
        
        {
            // attempt to pull a data off of the queue
            // if we cant, then wait on the condition var
            std::unique_lock<std::mutex> lk{ mJobLock };
            std::cout << "ThreadPool: Thread " << id << ": Going to sleep on CV wait" << std::endl;
            mJobSignal.wait(lk);
            std::cout << "ThreadPool: Thread " << id << ": Woken" << std::endl;


            // if we were waken up but the queue is still empty
            // then loop and continue waiting.
            // otherwise remove a job from the queue
            if (mJobs.empty()) {
                std::cout << "ThreadPool: Thread " << id << ": No jobs to process" << std::endl;
                continue;
            } else {
                job = mJobs.front();
                mJobs.pop();
                std::cout << "ThreadPool: Thread " << id << ": Job received" << std::endl;
            }
        }
       
        std::cout << "ThreadPool: Thread " << id << " Image processing started" << std::endl;
        ThreadPoolTypes::process_job(job);
        std::cout << "ThreadPool: Thread " << id << " Image processed" << std::endl;
    }
}