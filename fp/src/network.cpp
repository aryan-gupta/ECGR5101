#include "network.hpp"

#include <array>
#include <iterator>
#include <string>
#include <iostream>

#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

#include <opencv2/opencv.hpp>

#include "base64.h"

cv::Mat receive_image(int socket) {
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
    std::cout << "NETWRK: Image with len: " << lengthToReceive << " receiving" << std::endl;

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

    std::cout << "NETWRK: Received image" << std::endl;

    // return the decoded image
    return image;
}


void send_image(int socket, cv::Mat img) {
    // convert the image to base64
    std::vector<uchar> buf;
    cv::imencode(".png", img, buf);
    auto base64_png = reinterpret_cast<const unsigned char*>(buf.data());
    std::string encoded_png = base64_encode(base64_png, buf.size());

    // construct the message
    std::string message = std::to_string(encoded_png.length()) + "\n" + encoded_png;

    // send the message
    send(socket, message.data(), message.length(), 0 );
    
    // debugging symbols
    std::cout << "NETWRK: Image was sent" << std::endl;
    // std::cout << encoded_png.substr(0, 15) << std::endl;
    // std::cout << encoded_png.substr(encoded_png.size() - 15, encoded_png.size()) << std::endl;

    // std::cout << "Full Len: " << message.length() << std::endl;
    std::cout << "NETWRK: Image Len: " << encoded_png.length() << std::endl;
}