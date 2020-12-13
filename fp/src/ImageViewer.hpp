#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <opencv2/core/mat.hpp>

/// This class shows the image to the user. Because QT will
/// cause a deadlock if the first call to imshow is not done
/// by the same thread as the second call
class ImageViewer {
public:
    /// A finished job. Holds the original image
    /// and the grayscale image
    struct FinishedJob {
        cv::Mat image;
        cv::Mat gray;
    };

private:
    /// The thread for this class
    std::thread mThread;

    /// The protection lock for the image queue
    std::mutex mLock;
    /// Signals this thread that more images are
    /// ready for viewing
    std::condition_variable mCV;
    /// Stores a queue of image to show
    std::queue<FinishedJob> mImages;

    /// The thread target. This is what mThread runs
    void thread_target();

public:
    ImageViewer() = default;

    /// Stops this thread
    void stop();

    /// Starts this thread
    void run();

    /// Signals the thread that more images is ready to view
    /// @note calls to this are thread-safe
    void signal(FinishedJob &fd);
};