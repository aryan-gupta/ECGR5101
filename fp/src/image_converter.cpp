#include "image_converter.hpp"

#include <thread>
#include <chrono>

#include <opencv2/opencv.hpp>

cv::Mat convert_to_grayscale(cv::Mat in) {
    // create an empty image of the same size and
    // type
    cv::Mat ret{ 
        in.rows,
        in.cols,
        in.type()
    };

    // convert the color to grayscale
    cv::cvtColor(in, ret, cv::COLOR_RGB2GRAY);

    // sleep for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds{ 10 });

    return ret;
}