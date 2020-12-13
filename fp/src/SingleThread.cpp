#include "SingleThread.hpp"

#include <iostream>

#include "main.hpp"
#include "image_converter.hpp"
#include "network.hpp"

SingleThread::SingleThread()
    : ThreadPoolTypes{  } {  }

void SingleThread::stop() {
    return;
}

void SingleThread::addJob(int job) {
    std::cout << "SingleThread: Image processing started" << std::endl;
    ThreadPoolTypes::process_job(job);
    std::cout << "SingleThread: Image processed, exiting" << std::endl;
}