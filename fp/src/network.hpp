#pragma once

#include <opencv2/core/mat.hpp>

cv::Mat receive_image(int socket);
void send_image(int socket, cv::Mat image);