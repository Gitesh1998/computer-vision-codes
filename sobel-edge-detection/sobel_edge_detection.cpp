#include <opencv2/highgui.hpp>  // Include the header for high-level GUI functions
#include <opencv2/imgproc.hpp>  // Include the header for image processing functions
#include <iostream>             // Include the header for standard input/output stream objects

using namespace cv;             // Use the OpenCV namespace for easier code writing
using namespace std;            // Use the standard namespace for easier code writing

int main(int argc, char** argv) {
    // Check if the image file name is provided as a command line argument
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <image_file>" << endl;  // Print usage message if no file name is provided
        return -1;  // Exit the program with an error code
    }

    // Read the input image in grayscale mode
    Mat inputImage = imread(argv[1], IMREAD_GRAYSCALE);

    // Check if the image was loaded successfully
    if (inputImage.empty()) {
        cerr << "Error: Could not read the image." << endl;  // Print error message if the image is not loaded
        return -1;  // Exit the program with an error code
    }

    // Apply Gaussian blur to reduce noise and improve edge detection
    // The Size(5, 5) specifies a 5x5 kernel for the blur operation
    // The 1.4 parameter specifies the standard deviation of the Gaussian kernel
    GaussianBlur(inputImage, inputImage, Size(5, 5), 1.4);

    // Initialize matrices for storing the results of the Sobel operations
    Mat edgesX, edgesY, edges;

    // Perform the Sobel edge detection in the X direction
    // CV_16S specifies 16-bit signed integer depth for the Sobel output
    // 1, 0 indicates the Sobel kernel for X-direction gradients
    // 3 specifies the size of the Sobel kernel (3x3)
    Sobel(inputImage, edgesX, CV_16S, 1, 0, 3);

    // Perform the Sobel edge detection in the Y direction
    // 0, 1 indicates the Sobel kernel for Y-direction gradients
    Sobel(inputImage, edgesY, CV_16S, 0, 1, 3);

    // Convert the 16-bit signed Sobel edge images to 8-bit images
    // This conversion is necessary for visualization
    convertScaleAbs(edgesX, edgesX);
    convertScaleAbs(edgesY, edgesY);

    // Combine the X and Y edge results into a single edge map
    // addWeighted performs a weighted sum of the two edge images
    // 0.5 and 0.5 are the weights for X and Y directions, respectively
    // 0 is the scalar added to the result (0 in this case)
    addWeighted(edgesX, 0.5, edgesY, 0.5, 0, edges);

    // Display the original grayscale image
    imshow("Original Image", inputImage);

    // Display the result of the Sobel edge detection
    imshow("Sobel Edge Detection", edges);

    // Wait for a key press indefinitely
    waitKey(0);

    return 0;  // Return success code
}
