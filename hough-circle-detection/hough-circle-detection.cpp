#include <unistd.h>                // Header for POSIX API, for standard symbolic constants and types
#include <stdio.h>                 // Header for input and output operations
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
    namedWindow("Capture Example", WINDOW_AUTOSIZE); // Create a window for display
    VideoCapture capture;                            // Create a VideoCapture object to capture video from a device
    Mat frame, gray;                                 // Declare matrices to hold the frames and grayscale images
    vector<Vec3f> circles;                           // Declare a vector to hold circle parameters

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
        cout << "usage: capture [dev]" << endl;      // Print usage instructions
        exit(-1);                                    // Exit with error code -1
    }

    capture.open(dev);                               // Open the video capture with the specified device ID
    capture.set(CAP_PROP_FRAME_WIDTH, HRES);         // Set the frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, VRES);        // Set the frame height

    while (1)                                        // Infinite loop to continuously capture frames
    {
        capture >> frame;                            // Capture a new frame

        if (frame.empty())                           // If the frame is empty (end of video or error)
            break;                                   // Exit the loop

        Mat mat_frame(frame);                        // Convert the captured frame to a Mat object

        cvtColor(mat_frame, gray, COLOR_BGR2GRAY);   // Convert the frame to grayscale

        GaussianBlur(gray, gray, Size(9, 9), 2, 2);  // Apply Gaussian blur to reduce noise

        // Detect circles in the grayscale image using the Hough Circle Transform
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 8, 100, 50, 0, 0);

        printf("circles.size = %ld\n", circles.size()); // Print the number of circles detected

        for (size_t i = 0; i < circles.size(); i++)  // Loop through all detected circles
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1])); // Get the center of the circle
            int radius = cvRound(circles[i][2]);    // Get the radius of the circle
            // Draw the circle center
            circle(mat_frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
            // Draw the circle outline
            circle(mat_frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
        }

        imshow("Capture Example", mat_frame);        // Display the frame with detected circles

        char c = waitKey(10);                        // Wait for 10 milliseconds for a key press
        if (c == 'q')                                // If the 'q' key is pressed
            break;                                   // Exit the loop
    }

    return 0;                                        // Return success code 0
}
