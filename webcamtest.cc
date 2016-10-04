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

void getFrame(Mat& gray, vector<KeyPoint>& kp, Mat& desc)
{
    Mat frame;
    camera >> frame;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    //Ptr<Feature2D> orb = ORB::create(1000, 1.2, 8, 15, 0, 2, ORB::HARRIS_SCORE, 15, 15);
    Ptr<Feature2D> orb = ORB::create();
    orb->detectAndCompute(gray, Mat(), kp, desc);
}

bool sortfn(DMatch &a, DMatch &b)
{
    return a.distance - b.distance;
}

int main(int argc, char **argv)
{
    // OpenCV prints a huge number of JPEG errors to stderr when reading from a webcam
    fclose(stderr);

    if (!camera.isOpened())
    {
        cout << "Failed to open video" << endl;
        return 1;
    }


    namedWindow(win);

    Mat prev_frame, prev_desc;
    vector<KeyPoint> prev_kp;
    getFrame(prev_frame, prev_kp, prev_desc);

    while (true)
    {

        vector<KeyPoint> kp;
        vector<vector<DMatch>> matches;
        Mat frame, gray, frame_final, desc;

        getFrame(frame, kp, desc);

        float dx = 0, dy = 0;
        int samples = 0;

        if (!desc.empty() && !prev_desc.empty())
        {
            BFMatcher matcher;
            matcher.knnMatch(desc, prev_desc, matches, 2);

            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i][0].distance <= matches[i][1].distance * 0.5)
                {
                    Point2f from = kp[matches[i][0].queryIdx].pt;
                    Point2f to = prev_kp[matches[i][0].trainIdx].pt;
                    line(frame, to, from, Scalar::all(255));

                    // Running average
                    Point2f diff = from - to;
                    dx += diff.x;
                    dy += diff.y;
                    samples++;
                }
            }

            if (samples)
            {
                dx /= samples;
                dy /= samples;
                if (dx < 1.0 && dx > -1.0)
                {
                    dx = 0.0;
                }
                if (dy < 1.0 && dy > -1.0)
                {
                    dy = 0.0;
                }
            }


            stringstream ss;
            ss.precision(2);
            ss << "dx: " << dx;
            putText(frame, ss.str(), Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 0));
            ss.str("");
            ss << "dy: " << dy;
            putText(frame, ss.str(), Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 0));
            imshow(win, frame);
        }
        prev_frame = frame;
        prev_kp = kp;
        prev_desc = desc;
       
        waitKey(25);

    }

    return 0;
}
