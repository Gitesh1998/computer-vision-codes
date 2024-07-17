#include <opencv2/opencv.hpp> // Include the OpenCV library header for all necessary OpenCV functions
#include <iostream>           // Include the header for standard input/output stream objects

using namespace cv;           // Use the OpenCV namespace for easier code writing
using namespace std;          // Use the standard namespace for easier code writing

int main(int argc, char **argv) // Main function taking command-line arguments
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <video-file>" << endl;
        return -1;
    }

    string videoFile = argv[1];
    VideoCapture vcap;         // Create a VideoCapture object for capturing video from a file or camera
    Mat mat_frame;             // Declare a matrix to hold each video frame
    if (!vcap.open(videoFile)) // Attempt to open the specified video file
    {
        cout << "Error opening video stream or file" << endl; // Print error message if the file cannot be opened
        return -1;                         // Exit the program with an error code
    }

    int frame_count = 1;                 // Initialize frame count to 1

    while (1)                            // Infinite loop to continuously capture and process frames
    {
        if (!vcap.read(mat_frame))       // Read the next frame into mat_frame
        {
            cout << "No frame" << endl;  // Print error message if no frame is captured
            break;                       // Exit the loop if no frame is captured
        }
        resize(mat_frame, mat_frame, Size(640, 480)); // Resize the frame to 640x480 resolution

        int totalPoint = 0;              // Initialize totalPoint to count points above the threshold
        int massX = 0;                   // Initialize massX to accumulate x-coordinates of points above the threshold
        int massY = 0;                   // Initialize massY to accumulate y-coordinates of points above the threshold
        Mat grayImage(mat_frame.rows, mat_frame.cols, CV_8UC1); // Create a grayscale image with the same dimensions as the frame
        uchar threshold = 100;           // Set the threshold value for detecting bright spots
        if (frame_count > -1)            // Process the frame if frame_count is valid
        {
            for (int y = 0; y < mat_frame.rows; ++y) // Iterate over all rows of the frame
            {
                for (int x = 0; x < mat_frame.cols; ++x) // Iterate over all columns of the frame
                {
                    Vec3b pixel = mat_frame.at<Vec3b>(y, x); // Get the BGR pixel value at (x, y)
                    uchar grayVal = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]); // Convert BGR to grayscale using standard conversion formula
                    grayImage.at<uchar>(y, x) = 0; // Initialize the grayscale image pixel to 0
                    if (threshold < grayVal) // Check if the grayscale value is above the threshold
                    {
                        totalPoint++; // Increment the count of points above the threshold
                        massX += x; // Accumulate the x-coordinate of the point
                        massY += y; // Accumulate the y-coordinate of the point
                    }
                }
            }
        }

        int centerOfMassX = static_cast<int>(massX / totalPoint); // Calculate the x-coordinate of the center of mass
        int centerOfMassY = static_cast<int>(massY / totalPoint); // Calculate the y-coordinate of the center of mass

        for (int y = 0; y < mat_frame.rows; ++y) // Iterate over all rows to draw a vertical line at the center of mass
        {
            grayImage.at<uchar>(y, centerOfMassX) = 255; // Set the pixel at (centerOfMassX, y) to white
            if ((centerOfMassX - 1) > 0) // Check if the pixel to the left of the center is within bounds
            {
                grayImage.at<uchar>(y, centerOfMassX - 1) = 255; // Set the left pixel to white
            }

            if ((centerOfMassX + 1) < mat_frame.cols) // Check if the pixel to the right of the center is within bounds
            {
                grayImage.at<uchar>(y, centerOfMassX + 1) = 255; // Set the right pixel to white
            }
        }

        for (int x = 0; x < mat_frame.cols; ++x) // Iterate over all columns to draw a horizontal line at the center of mass
        {
            grayImage.at<uchar>(centerOfMassY, x) = 255; // Set the pixel at (x, centerOfMassY) to white
            if ((centerOfMassY - 1) > 0) // Check if the pixel above the center is within bounds
            {
                grayImage.at<uchar>(centerOfMassY - 1, x) = 255; // Set the above pixel to white
            }
            if ((centerOfMassY + 1) < mat_frame.rows) // Check if the pixel below the center is within bounds
            {
                grayImage.at<uchar>(centerOfMassY + 1, x) = 255; // Set the below pixel to white
            }
        }

        string filename = "frame" + to_string(frame_count) + ".pgm"; // Create a filename for the current frame
        imwrite(filename, grayImage); // Save the grayscale image as a PGM file
        frame_count++; // Increment the frame count

        imshow("Original", mat_frame); // Display the original frame in a window
        imshow("Frame", grayImage); // Display the processed grayscale image in a window
        char c = waitKey(33); // Wait for 33 milliseconds for a key press
        if (c == 'q') // If the 'q' key is pressed
            break; // Exit the loop
    }
}
