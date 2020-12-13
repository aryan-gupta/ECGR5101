#include "ThreadPoolPriority.hpp"

#include <thread>
#include <iostream>

#include <pthread.h>
#include <sched.h>

ThreadPoolPriority::ThreadPoolPriority(size_t numThreads)
    : ThreadPool{ numThreads }
{
    // Set the priorities
    pthread_attr_t tattr;
    sched_param param;
    int ret;
    int policy = SCHED_FIFO;

    ret = pthread_attr_init(&tattr);
    ret = pthread_attr_getschedparam(&tattr, &param);
    pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&tattr, policy);
    param.sched_priority = param.sched_priority - 1;
    ret = pthread_attr_setschedparam(&tattr, &param);

    for (auto &t : mThreads) {
        pthread_setschedparam(t.native_handle(), policy, &param);
    }

    std::cout << "ThreadPoolPriority: Priority of thread set" << std::endl;
}