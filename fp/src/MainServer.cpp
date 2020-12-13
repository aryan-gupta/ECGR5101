#include "MainServer.hpp"

#include <iostream>

#include <errno.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <fcntl.h>
#include <sys/select.h>

#include "main.hpp"

MainServer::MainServer(ThreadPoolTypes *const pool)
    : mThread{ }, mPool{ pool }, mStop{ false }
{  }

void MainServer::run(bool priority) {
    // start the thread
    mThread = std::thread{ &MainServer::thread_target, this };

    // assign the priority if we need to. Copied from the example
    if (priority) {
        pthread_attr_t tattr;
        sched_param param;
        int ret;
        int policy = SCHED_FIFO;

        ret = pthread_attr_init(&tattr);
        ret = pthread_attr_getschedparam(&tattr, &param);
        pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&tattr, policy);
        param.sched_priority = param.sched_priority + 1;
        ret = pthread_attr_setschedparam(&tattr, &param);

        pthread_setschedparam(mThread.native_handle(), policy, &param);
        std::cout << "MainServer: Priority changed" << std::endl;   
    }
}

void MainServer::stop() {
    mStop.store(true);
    mThread.join();
}

void MainServer::thread_target() const {
    std::cout << "MainServer: Main server started" << std::endl;

    bool quit = false;
    int server_fd, new_socket; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    // get socket flags
	// if ((opt = fcntl (server_fd, F_GETFL, NULL)) < 0) {
	// 	exit(EXIT_FAILURE);
	// }

	// // set socket non-blocking
	// if (fcntl (server_fd, F_SETFL, opt | O_NONBLOCK) < 0) {
	// 	exit(EXIT_FAILURE);
	// }

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Listen to on the addr and port
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    std::cout << "MainServer: Socket server setup. Waiting for connections" << std::endl;

    do {
        // if we get a new connection request
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket == -1)
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }

        std::cout << "MainServer: New connection received" << std::endl;
        
        // This is not needed, but I need to get non-blocking accept so
        // we can exit the server gracefully
        if (new_socket != EWOULDBLOCK || new_socket != EAGAIN) {
            // add the socket to the job list so the worker threads
            // can work on them
            mPool->addJob(new_socket);
            std::cout << "MainServer: Job added" << std::endl;
        }

        // reload quit to test if we need to exit the server
        quit = mStop.load();
    } while (!quit);
}