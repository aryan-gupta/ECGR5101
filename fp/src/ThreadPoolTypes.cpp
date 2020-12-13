#include "ThreadPoolTypes.hpp"

#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

#include <array>
#include <iterator>
#include <string>

#include <opencv2/opencv.hpp>

#include "base64.h"

ThreadPoolTypes::ThreadPoolTypes(ImageViewer &iv)
    : mViewer{ iv } {

}

cv::Mat ThreadPoolTypes::receive_image(int socket) {
    // master buffer for the entire image
    std::string master_buffer{ };
    // buffer to store the length of the message
    std::string lengthToReceiveStr{  };

    // reserve data plus some extra so we prevent unnecessary reallocations 
    lengthToReceiveStr.reserve(128 + 16);
    master_buffer.reserve(128 + 16);

    bool lengthFound = false;
    do {
        // raw buffer for the recv function
        std::array<char, 128> buffer{ };
        
        size_t numread = read(socket, buffer.data(), buffer.size());

        // go through each byte that we read and add it to the lengthToReceiveStr
        // if we find a \n line character than we know we have received the length
        // data, if so then copy the rest of the buffer to the master buffer
        // now lengthToReceiveStr has the length of the image and master_buffer
        // has some of the image data
        for (size_t i = 0; i < numread; ++i) {
            lengthToReceiveStr.push_back(buffer[i]);

            if (buffer[i] == '\n') {
                std::move(buffer.begin() + i + 1, buffer.begin() + numread, std::back_inserter(master_buffer));
                lengthFound = true;
                break;
            }
        }
    } while (!lengthFound);

    // parse the string length to an int
    size_t lengthToReceive = std::stoll(lengthToReceiveStr);
    std::cout << "Image with len: " << lengthToReceive << " receiving" << std::endl;

    // reserve more data so we dont do unnecessary reallocs
    master_buffer.reserve(lengthToReceive + 32);

    // if we haven't received the full image, keep receiving more
    // data until our master_buffer has the full image
    while (master_buffer.length() < lengthToReceive) {
        std::array<char, 1024 * 64> buffer{ };

        size_t numread = read(socket, buffer.data(), buffer.size());
        
        std::move(buffer.begin(), buffer.begin() + numread, std::back_inserter(master_buffer));
    }

    // decode the base64 string that we received to the actual image data 
    std::string decoded_img = base64_decode(master_buffer);
    std::vector<unsigned char> raw_img{ decoded_img.begin(), decoded_img.end() };
    cv::Mat image = cv::imdecode(raw_img, cv::IMREAD_COLOR);

    std::cout << "Received image" << std::endl;

    // return the decoded image
    return image;
}