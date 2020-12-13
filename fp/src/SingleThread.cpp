#include "SingleThread.hpp"

#include "main.hpp"
#include "image_converter.hpp"

SingleThread::SingleThread(ImageViewer &iv)
    : ThreadPoolTypes{ iv } {  }

void SingleThread::stop() {
    return;
}

void SingleThread::addJob(int job) {
    ImageViewer::FinishedJob fd{  };
    
    fd.image = receive_image(job);
    fd.gray = convert_to_grayscale(fd.image);

    // signal to the viewer that we have compleated
    // an image and its ready to show
    mViewer.signal(fd);
}