#include <opencv2/highgui.hpp>  // Include the OpenCV highgui header for GUI functions
#include <opencv2/imgproc.hpp>  // Include the OpenCV image processing header for image operations
#include <iostream>             // Include the iostream header for standard I/O operations

using namespace cv;             // Use the OpenCV namespace to avoid prefixing functions with 'cv::'
using namespace std;            // Use the standard namespace to avoid prefixing functions with 'std::'

int main(int argc, char** argv)
{
    // Check if the image file name is provided as an argument
    if (argc != 2)
    {
        // Print an error message if the argument count is not correct
        cerr << "Usage: " << argv[0] << " <image_path>" << endl;
        return -1;
    }

    // Read the input image in grayscale mode from the specified file path
    Mat inputImage = imread(argv[1], IMREAD_GRAYSCALE);
    // Display the original image in a window
    imshow("Original Image", inputImage);
    // Check if the image was successfully read
    if (inputImage.empty())
    {
        // Print an error message to standard error and exit the program with a non-zero status
        cerr << "Error: Could not read the image." << endl;
        return -1;
    }

    // Apply Gaussian blur to the input image to reduce noise and improve edge detection
    GaussianBlur(inputImage, inputImage, Size(5, 5), 1.4);

    // Create a matrix to hold the edges detected by the Canny algorithm
    Mat edges;
    // Apply the Canny edge detection algorithm to the blurred image
    Canny(inputImage, edges, 50, 150);

    // Display the edges detected by the Canny algorithm in a window
    imshow("Canny Edge Detection", edges);
    // Wait indefinitely until a key is pressed
    waitKey(0);

    // Exit the program with a zero status indicating success
    return 0;
}
