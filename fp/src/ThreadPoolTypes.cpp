#include "ThreadPoolTypes.hpp"

#include <iostream>

#include "network.hpp"
#include "image_converter.hpp"

void ThreadPoolTypes::process_job(int socket) {
    cv::Mat image = receive_image(socket);
    std::cout << "ThreadPoolTypes: Image received from socket" << std::endl;
    cv::Mat gray = convert_to_grayscale(image);
    std::cout << "ThreadPoolTypes: Image converted to grayscale" << std::endl;

    send_image(socket, gray);
    std::cout << "ThreadPoolTypes: Image sent back to client" << std::endl;

}
