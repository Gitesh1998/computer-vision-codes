#include <unistd.h>                // Header for POSIX API, providing access to the operating system API
#include <stdio.h>                 // Header for standard input and output functions
#include <stdlib.h>                // Header for general purpose standard library functions
#include <iostream>                // Header for standard input and output stream objects

#include <opencv2/core/core.hpp>   // Header for core functionalities of OpenCV
#include <opencv2/highgui/highgui.hpp> // Header for High-level GUI functionalities of OpenCV
#include <opencv2/imgproc/imgproc.hpp> // Header for image processing functionalities of OpenCV

using namespace cv;                // Use OpenCV's namespace for easier code writing
using namespace std;               // Use standard namespace for easier code writing

#define HRES 640                   // Define horizontal resolution
#define VRES 480                   // Define vertical resolution

int main(int argc, char** argv)
{
    namedWindow("Capture Example", WINDOW_AUTOSIZE); // Create a window for display with automatic size adjustment
    VideoCapture capture;                            // Create a VideoCapture object to capture video from a device
    Mat frame, gray, canny_frame, cdst;              // Declare matrices to hold frames, grayscale images, Canny edge detected images, and color images
    vector<Vec4i> lines;                             // Declare a vector to hold line parameters from Hough Transform

    int dev = 0;                                     // Default device ID is 0

    if (argc > 1)                                    // If there are more than one command-line arguments
    {
        sscanf(argv[1], "%d", &dev);                 // Convert the first argument to an integer (device ID)
        cout << "Using " << argv[1] << endl;         // Print the device ID being used
    }
    else if (argc == 1)                              // If there is only one command-line argument
    {
        cout << "Using default" << endl;             // Print that the default device is being used
    }
    else                                             // If no command-line arguments are provided
    {
        cout << "Usage: capture [dev]" << endl;      // Print usage instructions
        return -1;                                   // Return with error code -1
    }

    capture.open(dev);                               // Open the video capture with the specified device ID
    capture.set(CAP_PROP_FRAME_WIDTH, HRES);         // Set the frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, VRES);        // Set the frame height

    while (1)                                        // Infinite loop to continuously capture frames
    {
        capture >> frame;                            // Capture a new frame

        if (frame.empty())                           // If the frame is empty (end of video or error)
            break;                                   // Exit the loop

        // Apply Canny edge detection
        Canny(frame, canny_frame, 50, 200, 3);

        // Convert the Canny edge detected image to color
        cvtColor(canny_frame, cdst, COLOR_GRAY2BGR);
        
        // Convert the captured frame to grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Detect lines in the grayscale image using the Hough Line Transform
        HoughLinesP(canny_frame, lines, 1, CV_PI / 360, 50, 5, 2);

        // Loop through all detected lines and draw them on the frame
        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];                      // Extract the line parameters
            line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA); // Draw the line on the frame
        }

        imshow("Capture Example", frame);            // Display the frame with detected lines

        char c = waitKey(10);                        // Wait for 10 milliseconds for a key press
        if (c == 'q')                                // If the 'q' key is pressed
            break;                                   // Exit the loop
    }

    return 0;                                        // Return success code 0
}
