#include "SingleThread.hpp"

#include "main.hpp"
#include "image_converter.hpp"
#include "network.hpp"

SingleThread::SingleThread()
    : ThreadPoolTypes{  } {  }

void SingleThread::stop() {
    return;
}

void SingleThread::addJob(int job) {
    ThreadPoolTypes::process_job(job);
}