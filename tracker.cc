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
static Ptr<Feature2D> detector = AKAZE::create(AKAZE::DESCRIPTOR_KAZE);

void getFrame(Mat& gray, vector<KeyPoint>& kp, Mat& desc)
{
    Mat frame;
    camera >> frame;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    detector->detectAndCompute(gray, noArray(), kp, desc);
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

    Mat target = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE), target_desc;
    vector<KeyPoint> target_kp;

    detector->detectAndCompute(target, noArray(), target_kp, target_desc);

    while (true)
    {

        vector<KeyPoint> kp;
        vector<vector<DMatch>> matches;
        vector<vector<DMatch>> matches_final;
        Mat frame, frame_final, desc;

        getFrame(frame, kp, desc);

        if (!desc.empty())
        {
            BFMatcher matcher;
            matcher.knnMatch(desc, target_desc, matches, 2);

            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i][0].distance <= matches[i][1].distance * 0.8)
                {
                    matches_final.push_back(matches[i]);
                }
            }

            drawMatches(target, target_kp, frame, kp, matches_final, frame_final, Scalar(255, 0, 0), Scalar(255, 0, 0));
            imshow(win, frame_final);
        }

        waitKey(25);

    }

    return 0;
}
