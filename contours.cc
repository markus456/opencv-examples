#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <sstream>

using namespace cv;
using namespace std;

const char *win = "window1";
const int level = 20;

// Use 0 if you only have one camera on your computer
#define CAMERA_NUMBER 1

static VideoCapture camera(CAMERA_NUMBER);
static Ptr<Feature2D> detector = ORB::create();

void getFrame(Mat& orig, Mat& edges)
{
    Mat gray;
    camera >> orig;
    cvtColor(orig, gray, COLOR_BGR2GRAY);
    Canny(gray, edges, 80, 180);
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        cout << "Usage: tracker FILE" << endl;
        return 1;
    }

    // OpenCV prints a huge number of JPEG errors to stderr when reading from a webcam
    fclose(stderr);

    if (!camera.isOpened())
    {
        cout << "Failed to open video" << endl;
        return 1;
    }

    namedWindow(win);

    Mat target, target_color = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE), target_desc;
    vector<KeyPoint> target_kp;

    Canny(target_color, target, 80, 180);
    detector->detectAndCompute(target, noArray(), target_kp, target_desc);

    while (true)
    {
        vector<vector<Point>> contours;
        Mat frame, frame_final, edges;

        getFrame(frame, edges);
        
        findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        drawContours(frame, contours, -1, Scalar(255, 0, 0));
        imshow(win, frame);

        waitKey(25);

    }

    return 0;
}
