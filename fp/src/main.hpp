
#include <string>

#include <opencv2/core/mat.hpp>

constexpr uint16_t PORT = 4321;

void show_converted_grayscale_image(std::string path);
void wait_for_exit();
void show_images(cv::Mat normal, cv::Mat gray);