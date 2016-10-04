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
static Ptr<Feature2D> orb = ORB::create();

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        cout << "Usage: capture FILE" << endl;
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

    while (true)
    {
        Mat frame, gray;
        camera >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        imshow(win, gray);

        char key = waitKey(50);
        if (key == '\n')
        {
            imwrite(String(argv[1]), gray);
            break;
        }

    }

    return 0;
}
