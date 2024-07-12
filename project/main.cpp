#include <opencv2/imgcodecs.hpp>  // Include for reading and writing image files
#include <opencv2/highgui.hpp>   // Include for GUI functions like displaying windows
#include <opencv2/imgproc.hpp>   // Include for image processing functions
#include <opencv2/core.hpp>      // Include for core functionalities and data structures
#include <opencv2/objdetect.hpp> // Include for object detection functions like CascadeClassifier
#include <iostream>             // Include for standard input/output operations
#include <chrono>               // Include for time measurement operations
#include <numeric>             // Include for numeric operations like accumulate for averaging
#include <omp.h>               // Include for parallel processing with OpenMP

using namespace std;  // Standard namespace for standard functions and types
using namespace cv;   // OpenCV namespace for core OpenCV functions and types

// Function to perform Canny edge detection
Mat canny(Mat img)
{
    Mat gray_img;  // Create a matrix to hold the grayscale image
    cvtColor(img, gray_img, COLOR_RGB2GRAY);  // Convert the input image from RGB to grayscale
    Mat edges;  // Create a matrix to hold the edges detected by Canny
    Canny(gray_img, edges, 110, 120);  // Apply Canny edge detection with specified thresholds
    return edges;  // Return the edges detected in the image
};

// Function to calculate the average of a vector of floats
float vectorAverage(vector<float> input_vec)
{
    // Calculate the average of all elements in the vector
    float average = accumulate(input_vec.begin(), input_vec.end(), 0.0) / input_vec.size();
    return average;  // Return the calculated average
};

// Function to draw lines on the image based on the detected lines
void drawLines(Mat img, vector<Vec4f> lines, int thickness = 5)
{
    Scalar right_color = Scalar(0, 255, 0);  // Define the color for right lane lines (Green)
    Scalar left_color = Scalar(0, 255, 0);   // Define the color for left lane lines (Green)
    vector<float> rightSlope, leftSlope, rightIntercept, leftIntercept;  // Vectors to store slopes and intercepts for each lane

    // Loop through each detected line
    for (Vec4f line : lines)
    {
        float x1 = line[0];  // Get the x-coordinate of the start point of the line
        float y1 = line[1];  // Get the y-coordinate of the start point of the line
        float x2 = line[2];  // Get the x-coordinate of the end point of the line
        float y2 = line[3];  // Get the y-coordinate of the end point of the line
        float slope = (y1 - y2) / (x1 - x2);  // Calculate the slope of the line
        if (slope > 0.5)  // Check if the line is a right lane line (positive slope)
        {
            if (x1 > 500)  // Further filter lines based on x-coordinate to ensure they are within the region of interest
            {
                float yintercept = y2 - (slope * x2);  // Calculate the y-intercept of the line
                rightSlope.push_back(slope);  // Store the slope of the right lane line
                rightIntercept.push_back(yintercept);  // Store the y-intercept of the right lane line
            };
        }
        else if (slope < -0.5)  // Check if the line is a left lane line (negative slope)
        {
            if (x1 < 700)  // Further filter lines based on x-coordinate to ensure they are within the region of interest
            {
                float yintercept = y2 - (slope * x2);  // Calculate the y-intercept of the line
                leftSlope.push_back(slope);  // Store the slope of the left lane line
                leftIntercept.push_back(yintercept);  // Store the y-intercept of the left lane line
            };
        };
    };

    // Compute the average values for slopes and intercepts of left and right lanes
    float left_intercept_avg = vectorAverage(leftIntercept);
    float right_intercept_avg = vectorAverage(rightIntercept);
    float left_slope_avg = vectorAverage(leftSlope);
    float right_slope_avg = vectorAverage(rightSlope);

    // Calculate the x-coordinates for the start and end points of the left lane line
    int left_line_x1 = (int)round((0.65 * img.rows - left_intercept_avg) / left_slope_avg);
    int left_line_x2 = (int)round((img.rows - left_intercept_avg) / left_slope_avg);
    // Calculate the x-coordinates for the start and end points of the right lane line
    int right_line_x1 = (int)round((0.65 * img.rows - right_intercept_avg) / right_slope_avg);
    int right_line_x2 = (int)round((img.rows - right_intercept_avg) / right_slope_avg);

    Point line_vertices[1][4];  // Define the vertices of the polygon to represent the lane area
    line_vertices[0][0] = Point(left_line_x1, (int)round(0.65 * img.rows));  // Top left point of the left lane line
    line_vertices[0][1] = Point(left_line_x2, img.rows);  // Bottom left point of the left lane line
    line_vertices[0][2] = Point(right_line_x2, img.rows);  // Bottom right point of the right lane line
    line_vertices[0][3] = Point(right_line_x1, (int)round(0.65 * img.rows));  // Top right point of the right lane line
    const Point *inner_shape[1] = {line_vertices[0]};  // Create a pointer array for the polygon shape
    int n_vertices[] = {4};  // Define the number of vertices for the polygon
    int lineType = LINE_8;  // Set the line type for drawing
    fillPoly(img, inner_shape, n_vertices, 1, Scalar(255, 0, 0), lineType);  // Fill the lane area with a blue color
    line(img, Point(left_line_x1, (int)round(0.65 * img.rows)), Point(left_line_x2, img.rows), left_color, thickness);  // Draw the left lane line
    line(img, Point(right_line_x1, (int)round(0.65 * img.rows)), Point(right_line_x2, img.rows), right_color, thickness);  // Draw the right lane line
};

// Function to perform Hough Line Transform and draw lines on a new image
Mat hough_lines(Mat img, double rho, double theta, int threshold, double min_line_len, double max_line_gap)
{
    vector<Vec4f> lines;  // Vector to store detected lines
    Mat line_img(img.rows, img.cols, CV_8UC3, Scalar(0, 0, 0));  // Create an image to draw the detected lines
    HoughLinesP(img, lines, rho, theta, threshold, min_line_len, max_line_gap);  // Apply the Hough Line Transform to detect lines

    drawLines(line_img, lines);  // Draw the detected lines on the image
    return line_img;  // Return the image with the drawn lines
};

// Function to perform line detection with default Hough Transform parameters
Mat lineDetect(Mat img)
{
    return hough_lines(img, 1, CV_PI / 180, 50, 100, 100);  // Call hough_lines with default parameters for rho, theta, threshold, min_line_len, and max_line_gap
};

// Function to blend two images together with specified weights
Mat weighted_img(Mat img, Mat initial_img, double alpha = 0.8, double beta = 1.0, double gamma = 0.0)
{
    Mat weighted_img;  // Create a matrix to store the resulting blended image
    addWeighted(img, alpha, initial_img, beta, gamma, weighted_img);  // Perform the weighted sum of two images
    return weighted_img;  // Return the blended image
};

// Function to perform lane detection on the source image
Mat LaneDetection(Mat src)
{
    Mat color_masked, roi_img, canny_img, hough_img, final_img;  // Create matrices for various stages of processing
    Mat hls, yellowmask, whitemask, maskN, masked;  // Create matrices for color space conversion and masks

    cvtColor(src, hls, COLOR_RGB2HLS);  // Convert the source image from RGB to HLS color space
    inRange(hls, Scalar(100, 0, 90), Scalar(50, 255, 255), yellowmask);  // Create a mask for detecting yellow colors
    inRange(hls, Scalar(0, 70, 0), Scalar(255, 255, 255), whitemask);  // Create a mask for detecting white colors
    bitwise_or(yellowmask, whitemask, maskN);  // Combine yellow and white masks using a bitwise OR operation
    bitwise_and(src, src, masked, maskN = maskN);  // Apply the combined mask to the source image

    int x = masked.cols;  // Get the width of the masked image
    int y = masked.rows;  // Get the height of the masked image
    Point polygon_vertices[1][4];  // Define the vertices of the polygon for the region of interest
    polygon_vertices[0][0] = Point(0, y);  // Bottom left vertex of the polygon
    polygon_vertices[0][1] = Point(x, y);  // Bottom right vertex of the polygon
    polygon_vertices[0][2] = Point((int)round(0.55 * x), (int)round(0.6 * y));  // Top right vertex of the polygon
    polygon_vertices[0][3] = Point((int)round(0.45 * x), (int)round(0.6 * y));  // Top left vertex of the polygon
    const Point *polygons[1] = {polygon_vertices[0]};  // Create a pointer array for the polygon shape
    int n_vertices[] = {4};  // Define the number of vertices for the polygon
    Mat mask(y, x, CV_8UC1, Scalar(0));  // Create a mask for the region of interest
    int lineType = LINE_8;  // Set the line type for drawing
    fillPoly(mask, polygons, n_vertices, 1, Scalar(255, 255, 255), lineType);  // Fill the polygon area in the mask with white color
    Mat masked_image;  // Create a matrix to hold the masked image
    bitwise_and(masked, masked, masked_image, mask = mask);  // Apply the region of interest mask to the masked image
    canny_img = canny(masked_image);  // Perform Canny edge detection on the masked image
    hough_img = lineDetect(canny_img);  // Perform Hough Line Transform to detect lines
    final_img = weighted_img(hough_img, src);  // Blend the Hough lines image with the original image
    return final_img;  // Return the final image with lane markings
};

// Main function to handle video processing and feature detection
int main(int argc, char *argv[])
{
    // Check for the correct number of command line arguments
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <video_file_name> [--show] [--store <output_file_name>]" << endl;
        return -1;  // Exit if there are not enough arguments
    }

    // Load the video file
    VideoCapture cap(argv[1]);  // Open the video file specified by the command line argument

    if (!cap.isOpened())
    {
        cout << "Error: Unable to open video file." << endl;
        return -1;  // Exit if the video file cannot be opened
    }

    // Create a window for displaying intermediate results if --show flag is provided
    bool showIntermediate = false;  // Flag to indicate if intermediate results should be shown
    string intermediateWindowName = "Intermediate Results";  // Name of the window for intermediate results
    if (argc > 2 && string(argv[2]) == "--show")
    {
        showIntermediate = true;  // Set the flag to show intermediate results
        namedWindow(intermediateWindowName, WINDOW_AUTOSIZE);  // Create a window for displaying intermediate results
    }

    // Create a video writer if --store flag is provided
    bool storeResults = false;  // Flag to indicate if results should be saved to a file
    VideoWriter outputVideo;  // VideoWriter object for saving the processed video
    string outputFileName;  // Variable to store the output file name
    if (argc > 3 && string(argv[3]) == "--store" && argc > 4)
    {
        storeResults = true;  // Set the flag to store results
        outputFileName = argv[4];  // Get the output file name from command line arguments
        int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // Define the codec for video writing
        Size frameSize = Size((int)cap.get(CAP_PROP_FRAME_WIDTH), (int)cap.get(CAP_PROP_FRAME_HEIGHT));  // Get the frame size of the video
        outputVideo.open(outputFileName, codec, cap.get(CAP_PROP_FPS), frameSize, true);  // Open the video writer with the specified codec and frame size
    }

    bool showNormalFrames = false;  // Flag to indicate if normal frames or processed frames should be shown
    cout << "Press space to turn on or turn off the features." << endl;  // Inform the user about the space bar functionality

    // Load the Haar Cascade classifier XML files for object detection
    CascadeClassifier pedestrianDetector;  // Create a CascadeClassifier object for pedestrian detection
    pedestrianDetector.load("./xmlfile/pedestrian1.xml");  // Load the pedestrian detection XML file

    CascadeClassifier carDetector;  // Create a CascadeClassifier object for car detection
    carDetector.load("./xmlfile/carDetection.xml");  // Load the car detection XML file

    CascadeClassifier trafficLightDetector;  // Create a CascadeClassifier object for traffic light detection
    trafficLightDetector.load("./xmlfile/traffic_light2.xml");  // Load the traffic light detection XML file

    // Framerate calculation
    auto start = std::chrono::steady_clock::now();  // Get the current time for calculating frame rate
    int fps = 0;  // Counter for frames per second
    int currentFps = 1;  // Variable to store the current frames per second value

    // Classifier array
    vector<CascadeClassifier> detectors = {pedestrianDetector, carDetector, trafficLightDetector};  // Store all the detectors in a vector

    // Process each frame of the video
    Mat frame;  // Matrix to store the current frame of the video
    while (cap.read(frame))  // Read frames from the video file
    {
        // Lane detection
        frame = LaneDetection(frame);  // Perform lane detection on the current frame

        vector<vector<Rect>> detection(3);  // Vector to store detected objects for each classifier

        #pragma omp parallel for  // Parallelize the object detection to improve performance
        for (int i = 0; i < 3; ++i)
        {
            detectors[i].detectMultiScale(frame, detection[i], 1.1, 2);  // Detect objects using each classifier
        }

        // Draw rectangles for detected objects

        for (const auto &rect : detection[2])  // Loop through detected traffic lights
        {
            rectangle(frame, rect.tl(), rect.br(), Scalar(0, 0, 255), 2);  // Draw red rectangles around detected traffic lights
        }

        for (const auto &rect : detection[1])  // Loop through detected cars
        {
            rectangle(frame, rect.tl(), rect.br(), Scalar(0, 255, 255), 2);  // Draw yellow rectangles around detected cars
        }

        for (const auto &rect : detection[0])  // Loop through detected pedestrians
        {
            rectangle(frame, rect.tl(), rect.br(), Scalar(128, 0, 128), 2);  // Draw purple rectangles around detected pedestrians
        }

        if (storeResults)  // Check if results should be saved
        {
            outputVideo.write(frame);  // Write the processed frame to the output video file
        }

        auto now = std::chrono::steady_clock::now();  // Get the current time for calculating frame rate
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();  // Calculate the elapsed time in seconds
        fps++;  // Increment the frame count
        putText(frame, "Frames/second: " + to_string(currentFps), Point(30, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 255, 0), 2);  // Display the current frame rate on the image
        if (duration >= 1)  // If one second has passed
        {
            currentFps = fps;  // Update the frame rate value
            fps = 0;  // Reset the frame count
            start = std::chrono::steady_clock::now();  // Reset the start time for the next second
        }
        imshow("Object Detection", frame);  // Display the current frame in the window
        const char key = (char)waitKey(1);  // Wait for a key press for 1 millisecond
        if (key == 27 || key == 'q')  // Check if the escape key or 'q' key is pressed
        {
            cout << "Exit requested" << endl;  // Inform the user that the program is exiting
            cap.release();  // Release the video capture object
            if (storeResults)
                outputVideo.release();  // Release the video writer object if results are being saved
            if (showIntermediate)
                destroyWindow(intermediateWindowName);  // Destroy the intermediate results window if it was created
            return 0;  // Exit the program
        }
        else if (key == ' ')  // Check if the space bar is pressed
        {
            cout << "Data: " << showNormalFrames << endl;  // Print the current state of feature detection
            if (showNormalFrames)
            {
                showNormalFrames = 0;  // Stop showing normal frames
                cout << "Feature Detection Started" << endl;  // Inform the user that feature detection is now started
            }
            else
            {
                showNormalFrames = 1;  // Start showing normal frames
                cout << "Features detection stopped" << endl;  // Inform the user that feature detection is now stopped
            }
        }
    }
}
