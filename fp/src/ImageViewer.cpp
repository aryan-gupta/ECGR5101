#include "ImageViewer.hpp"

#include "main.hpp"

void ImageViewer::stop() {
    return;
}

void ImageViewer::run() {
    mThread = std::thread{ &ImageViewer::thread_target, this };
}

void ImageViewer::signal(FinishedJob &fd) {
    {
        // lock the mutex and add the image to the
        // queue
        std::unique_lock<std::mutex> lk{ mLock };
        mImages.push(std::move(fd));
    }
    // notify the thread to display it
    mCV.notify_one();
}

void ImageViewer::thread_target() {
    while (true) {
        FinishedJob fd;
        
        {
            // query the queue to see if we have image to receive
            std::unique_lock<std::mutex> lk{ mLock };
            mCV.wait(lk);
            if (mImages.empty()) {
                // restart the loop if we dont
                continue;
            } else {
                fd = mImages.front();
                mImages.pop();
            }
        }

        // show the image if we do
        show_images(fd.image, fd.gray);
    }
}