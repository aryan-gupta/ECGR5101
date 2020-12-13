
#include <iostream>

#include <opencv2/opencv.hpp>

#include "main.hpp"
#include "image_converter.hpp"
#include "MainServer.hpp"
#include "client.hpp"
#include "SingleThread.hpp"
#include "MultiThreaded.hpp"
#include "ThreadPool.hpp"
#include "ThreadPoolPriority.hpp"

/// The test image to use during testing
const static std::string TEST_IMAGE_PATH{ "test/Lenna.png" };

int main() {
    using std::cout;
    using std::endl;
    using std::cin;

    cout << "\e[33mRemember to run `bspc rule -a \"*:*:Lenna\" state=tiled`\e[0m" << endl << endl;

    bool quit = false;
    do {
        cout << "Choose and option: " << endl;
        cout << "a. Start simple client" << endl;
        cout << "1. Convert image to grayscale" << endl;
        cout << "2. Start simple server" << endl;
        cout << "3. Start threaded server" << endl;
        cout << "4. Start thread pool server" << endl;
        cout << "5. Start priority thread pool server" << endl;
        cout << "q. Quit" << endl;

        cout << "\e[33;5mchoice? ::\e[0m ";
        
        std::string input;

        std::getline(cin, input);

        if (input == "a") {
            client_send_data(TEST_IMAGE_PATH);
            return 0;
        } else if (input == "q") {
            return 0;
        } else if (input == "1") {
            show_converted_grayscale_image(TEST_IMAGE_PATH);
            return 0;
        } else if (input == "2") {
            SingleThread worker{  };
            MainServer svr{ &worker };
            svr.run(false);
            wait_for_exit();
            svr.stop();
            return 0;
        } else if (input == "3") {
            MultiThreaded worker{  };
            MainServer svr{ &worker };
            svr.run(false);
            wait_for_exit();
            svr.stop();
            return 0;
        } else if (input == "4") {
            ThreadPool worker{  };
            MainServer svr{ &worker };
            svr.run(false);
            wait_for_exit();
            svr.stop();
            return 0;
        } else if (input == "5") {
            ThreadPoolPriority worker{  };
            MainServer svr{ &worker };
            svr.run(true);
            wait_for_exit();
            svr.stop();
            return 0;
        } else {
            cout << "\e[01;31mIncorrect option chosen, please try again.\e[0m" << endl;
            continue;
        }
    } while (!quit);

    return 0;
}

/// Simple shows a converted image using the path to the
/// image passed as a param
void show_converted_grayscale_image(std::string path) {
    using namespace cv;
    using namespace std;

    // Read the image file
    Mat image = imread(path);
    Mat gray  = convert_to_grayscale(imread(path));

    // Check for failure
    if (image.empty()) 
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return;
    }

    show_images(image, gray);
}

/// Spins and waits for the user to say they want to exit
void wait_for_exit() {
    using std::cout;
    using std::endl;
    using std::cin;

    do {
        cout << "\e[33;5mexit? ::\e[0m ";

        std::string input;
        std::getline(cin, input);

        if (input[0] == 'y') {
            return;
        }
    } while (true);
}

/// Shows the images.
/// @warning This function must always be called from the
///          same thread or QT (the window manager backend)
///          will have deadlock
/// @note To remedy the warning statement, this is only called
///       by the ImageViewer class. I should move this as a
///       member function
/// @todo Move this function to ImageViewer class
void show_images(cv::Mat normal, cv::Mat gray) {
    cv::String windowName = "Lenna"; //Name of the window
    cv::namedWindow(windowName); // Create a window
    cv::imshow(windowName, normal); // Show our image inside the created window.
    cv::waitKey(0);

    cv::imshow(windowName, gray); // Show our image inside the created window.
    cv::waitKey(0); // Wait for any keystroke in the window
    cv::destroyWindow(windowName); //destroy the created window
}