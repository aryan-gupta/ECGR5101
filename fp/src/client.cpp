#include "client.hpp"

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include <opencv2/opencv.hpp>

#include "base64.h"

#include "main.hpp"
#include "network.hpp"

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

    send_image(sock, img);
    cv::Mat gray = receive_image(sock);

    show_images(img, gray);

    return; 
}