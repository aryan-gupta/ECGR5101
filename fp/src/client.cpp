#include "client.hpp"

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include <opencv2/opencv.hpp>

#include "base64.h"

#include "main.hpp"

void client_send_data(std::string path) {
    using namespace std::string_literals;

    int sock; 
    struct sockaddr_in serv_addr; 

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
    } 
   
    /// connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return; 
    } 

    // load the image
    cv::Mat img = cv::imread(path);

    // convert the image to base64
    std::vector<uchar> buf;
    cv::imencode(".png", img, buf);
    auto base64_png = reinterpret_cast<const unsigned char*>(buf.data());
    std::string encoded_png = base64_encode(base64_png, buf.size());

    // construct the message
    std::string message = std::to_string(encoded_png.length()) + "\n" + encoded_png;

    // send the message
    send(sock, message.data(), message.length(), 0 );
    
    // debugging symbols
    std::cout << "Image was sent" << std::endl;
    std::cout << encoded_png.substr(0, 15) << std::endl;
    std::cout << encoded_png.substr(encoded_png.size() - 15, encoded_png.size()) << std::endl;

    std::cout << "Full Len: " << message.length() << std::endl;
    std::cout << "IMG  Len: " << encoded_png.length() << std::endl;

    return; 
}