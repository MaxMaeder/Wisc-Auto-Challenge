#include "constants.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/**
 * Generates regression line from points and draws a red fit line
*/
void drawFitLine(Mat &img, vector<Point> points) {
    Vec4f line;
    fitLine(points, line, DIST_L2, 0, 0.01, 0.01);

    float vx = line[0];
    float vy = line[1];
    float x0 = line[2];
    float y0 = line[3];
    float m = 10000.;
    cv::line(img, Point(x0-m*vx, y0-m*vy), Point(x0+m*vx, y0+m*vy), CV_RGB(255, 0, 0), 5);
}

int main() {

    // Read provided image
    string image_path = samples::findFile("red.png");
    Mat rawImg = imread(image_path, IMREAD_COLOR);
    if (rawImg.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Convert image color space
    Mat img;
    cvtColor(rawImg, img, COLOR_RGB2YCrCb);

    // Extract red channel from image
    Mat redImg;
    extractChannel(img, redImg, 2);
    //imshow("Red Image Channel", redImg);

    // Apply threshold to isolate really-red objects
    Mat binaryImg;
    threshold(redImg, binaryImg, 200, 255, THRESH_BINARY);
    //imshow("Applied Threshold", binaryImg);

    // Find contours in image
    vector<vector<Point>> contours;
    findContours(binaryImg, contours, RETR_LIST, CHAIN_APPROX_NONE);

    // Iterate over contours
    Mat contourImg = rawImg.clone();
    vector<Point> leftPoints;
    vector<Point> rightPoints;
    for (int i = 0; i < contours.size(); i++) {

        // Generate a bounding box around the contour
        Rect rect = boundingRect(contours[i]);

        // Throw out contours with too small an area
        if (contours[i].size() < MIN_CONTOUR_SIZE)
            continue;

        // Throw out contours that are too squat (can't be cones)
        if (rect.y < MIN_CONTOUR_HEIGHT)
            continue;

        // Find center of contour
        Point center = (rect.br() + rect.tl()) * 0.5;
        bool leftLine = center.x < contourImg.size().width / 2;
        if (leftLine) {
            leftPoints.push_back(center);
        } else {
            rightPoints.push_back(center);
        }

        // Draw rectangle around cone
        rectangle(contourImg, rect, leftLine ? CV_RGB(255, 255, 0) : CV_RGB(0, 255, 0), 5);
    }
    //imshow("Detected Contours", contourImg);

    // Draw regression lines
    Mat finalImg = rawImg.clone();
    drawFitLine(finalImg, leftPoints);
    drawFitLine(finalImg, rightPoints);
    //imshow("Final Detection", finalImg);

    imwrite("answer.png", finalImg);

    // Wait for a keystroke in the window
    // waitKey(0); 
    return 0;
}