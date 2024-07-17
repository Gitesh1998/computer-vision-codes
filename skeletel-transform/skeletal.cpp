#include <opencv2/highgui.hpp>  // Include the header for high-level GUI functions
#include <opencv2/imgproc.hpp>  // Include the header for image processing functions
#include <iostream>             // Include the header for standard input/output stream objects

using namespace cv;             // Use the OpenCV namespace for easier code writing
using namespace std;            // Use the standard namespace for easier code writing

// Main function
int main(int argc, char** argv)
{
    VideoCapture cap(0); // Open the default camera (camera 0)
    if (!cap.isOpened()) // Check if the camera is opened successfully
    {
        cerr << "Error: Unable to open camera!" << endl; // Print error message
        return -1; // Exit the program with an error code
    }

    cout << "Press 'q' or <ESC> to quit." << endl; // Print message for quitting

    Mat frame, gray, binary, mfblur; // Declare matrices to hold different stages of image processing
    int frame_count = 1; // Initialize frame counter

    while (true) // Infinite loop to process each frame
    {
        cap >> frame; // Capture a frame from the camera

        if (frame.empty()) // Check if the frame is empty
        {
            cerr << "Error: Unable to capture frame!" << endl; // Print error message
            break; // Exit the loop
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY); // Convert the captured frame to grayscale

        threshold(gray, binary, 50, 255, THRESH_BINARY); // Apply binary thresholding
        binary = 255 - binary; // Invert the binary image

        // To remove median filter, just replace blur value with 1
        medianBlur(binary, mfblur, 1); // Apply median blur to the binary image

        Mat skel(mfblur.size(), CV_8UC1, Scalar(0)); // Initialize the skeleton image
        Mat temp; // Temporary matrix for intermediate results
        Mat eroded; // Matrix to hold the eroded image
        Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5)); // Structuring element for morphological operations
        bool done; // Flag to indicate if skeletonization is complete
        int iterations = 0; // Counter for the number of iterations

        do
        {
            erode(mfblur, eroded, element); // Erode the image
            dilate(eroded, temp, element); // Dilate the eroded image
            subtract(mfblur, temp, temp); // Subtract the dilated image from the original image
            bitwise_or(skel, temp, skel); // Combine the result with the skeleton image
            eroded.copyTo(mfblur); // Copy the eroded image back to the original image

            done = (countNonZero(mfblur) == 0); // Check if there are no non-zero pixels left
            iterations++; // Increment the iteration counter
        } while (!done && (iterations < 100)); // Continue until skeletonization is complete or maximum iterations reached

        cout << "iterations=" << iterations << endl; // Print the number of iterations

        imshow("source", frame); // Display the original frame
        imshow("skeleton", skel); // Display the skeleton image

        string filename = "frame" + to_string(frame_count) + ".pgm"; // Generate the filename for saving the frame
        imwrite(filename, skel); // Save the skeleton image
        frame_count++; // Increment the frame counter
        if(frame_count > 4000) // If the frame counter exceeds 4000, exit the loop
        {
            break; // Exit the loop
        }

        char key = waitKey(30); // Wait for 30 milliseconds for a key press
        if (key == 27 || key == 'q') // If the 'ESC' or 'q' key is pressed
        {
            cout << "Exit requested" << endl; // Print message
            break; // Exit the loop
        }
    }

    cap.release();          // Release the camera
    destroyAllWindows();   // Close all OpenCV windows
    return 0; // Return success code
}
