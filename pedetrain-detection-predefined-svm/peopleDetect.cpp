#include <opencv2/objdetect.hpp>  // Include the header for object detection functions
#include <opencv2/highgui.hpp>    // Include the header for high-level GUI functions
#include <opencv2/imgproc.hpp>    // Include the header for image processing functions
#include <opencv2/videoio.hpp>    // Include the header for video input/output functions
#include <iostream>               // Include the header for standard input/output stream objects
#include <iomanip>                // Include the header for input/output manipulations

using namespace cv;               // Use the OpenCV namespace for easier code writing
using namespace std;              // Use the standard namespace for easier code writing

// Class to handle pedestrian detection using HOG descriptors
class Detector
{
    // Enumeration to define the mode of the detector
    enum Mode
    {
        Default,  // Default mode using the default people detector
        Daimler   // Daimler mode using the Daimler people detector
    } m;          // Variable to store the current mode

    // HOG descriptors for the default and Daimler people detectors
    HOGDescriptor hog, hog_d;

public:
    // Constructor to initialize the HOG descriptors and set the SVM detectors
    Detector() : m(Default), hog(), hog_d(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9)
    {
        hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());     // Set the default people detector
        hog_d.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());   // Set the Daimler people detector
    }

    // Toggle the mode between Default and Daimler
    void toggleMode() { m = (m == Default ? Daimler : Default); }

    // Return the name of the current mode as a string
    string modeName() const { return (m == Default ? "Default" : "Daimler"); }

    // Function to detect people in the input image
    vector<Rect> detect(InputArray img)
    {
        vector<Rect> found;  // Vector to store the detected rectangles
        if (m == Default)
            hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2, false);  // Default detection
        else if (m == Daimler)
            hog_d.detectMultiScale(img, found, 0.5, Size(8, 8), Size(32, 32), 1.05, 2, true); // Daimler detection
        return found;  // Return the detected rectangles
    }

    // Function to adjust the detected rectangles
    void adjustRect(Rect &r) const
    {
        r.x += cvRound(r.width * 0.1);     // Adjust x position
        r.width = cvRound(r.width * 0.8);  // Adjust width
        r.y += cvRound(r.height * 0.07);   // Adjust y position
        r.height = cvRound(r.height * 0.8);// Adjust height
    }
};

// Define command-line parser keys
static const string keys = "{ help h | | print help message }"
                           "{ camera c | 0 | capture video from camera (device index starting from 0) }"
                           "{ video v | | use video as input }";

// Main function
int main(int argc, char **argv)
{
    CommandLineParser parser(argc, argv, keys); // Create a command-line parser
    parser.about("This sample demonstrates the use of the HoG descriptor."); // About message for the parser
    if (parser.has("help")) // If help is requested
    {
        parser.printMessage(); // Print the help message
        return 0; // Exit the program
    }

    int camera = parser.get<int>("camera"); // Get the camera index from the command-line arguments
    string file = "video-file.mp4"; // Set the default video file

    if (!parser.check()) // Check for any errors in the command-line arguments
    {
        parser.printErrors(); // Print the errors
        return 1; // Exit the program with an error code
    }

    VideoCapture cap; // Create a VideoCapture object to capture video from a file or camera
    if (file.empty())
        cap.open(camera); // Open the camera if no file is specified
    else
    {
        file = samples::findFileOrKeep(file); // Find the specified video file
        cap.open(file); // Open the video file
    }

    if (!cap.isOpened()) // Check if the video capture is opened successfully
    {
        cout << "Can not open video stream: '" << (file.empty() ? "<camera>" : file) << "'" << endl; // Print error message
        return 2; // Exit the program with an error code
    }

    cout << "Press 'q' or <ESC> to quit." << endl; // Print message for quitting
    cout << "Press <space> to toggle between Default and Daimler detector" << endl; // Print message for toggling the detector

    Detector detector; // Create a Detector object
    Mat frame; // Create a matrix to hold each video frame

    for (;;) // Infinite loop to process each frame
    {
        cap >> frame; // Capture the next frame
        if (frame.empty()) // Check if the frame is empty
        {
            cout << "Finished reading: empty frame" << endl; // Print message if the frame is empty
            break; // Exit the loop
        }

        int64 t = getTickCount(); // Get the current tick count
        vector<Rect> found = detector.detect(frame); // Detect people in the frame
        t = getTickCount() - t; // Calculate the detection time

        // Display the detection mode and FPS on the frame
        {
            ostringstream buf; // Create a string buffer
            buf << "Mode: " << detector.modeName() << " ||| " // Append the detection mode
                << "FPS: " << fixed << setprecision(1) << (getTickFrequency() / (double)t); // Append the FPS
            putText(frame, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA); // Draw the text on the frame
        }

        for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i) // Iterate over all detected rectangles
        {
            Rect &r = *i; // Get the current rectangle
            detector.adjustRect(r); // Adjust the rectangle
            rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2); // Draw the rectangle on the frame
        }

        imshow("People detector", frame); // Show the frame in a window

        // Interact with the user
        const char key = (char)waitKey(30); // Wait for 30 milliseconds for a key press
        if (key == 27 || key == 'q') // If the 'ESC' or 'q' key is pressed
        {
            cout << "Exit requested" << endl; // Print message
            break; // Exit the loop
        }
        else if (key == ' ') // If the 'space' key is pressed
        {
            detector.toggleMode(); // Toggle the detection mode
        }
    }

    return 0; // Return success code
}
