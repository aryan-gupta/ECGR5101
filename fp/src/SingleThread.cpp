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
    cv::Mat image = receive_image(job);
    cv::Mat gray = convert_to_grayscale(image);

    send_image(job, gray);
}