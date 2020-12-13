#include "ThreadPoolTypes.hpp"

#include "network.hpp"
#include "image_converter.hpp"

void ThreadPoolTypes::process_job(int socket) {
    cv::Mat image = receive_image(socket);
    cv::Mat gray = convert_to_grayscale(image);

    send_image(socket, gray);
}
