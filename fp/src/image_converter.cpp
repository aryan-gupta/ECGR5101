#include "image_converter.hpp"

#include <thread>
#include <iostream>
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
    std::cout << "CVT2GRAY: Image converted to grayscale, sleeping" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{ 10 });
    std::cout << "CVT2GRAY: Simulated load compleate, waking up" << std::endl;

    return ret;
}