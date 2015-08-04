#pragma once
#include "main.hpp"

using namespace std;
using namespace cv;
using namespace cvb;

CvBlobs getBlobs(Mat *frame);           // get frame blob results

void getBlobMat(Mat *frame, 
                Mat* fore,
                CvBlobs blobs,         // get Mat(VIDEO_WIDTH * VIDEO_HEIGHT)
                vector<Mat>* images, Scalar& humanColor);   // blob in blobs respectively.

Scalar getFrameDominantColor(Mat *frame,   // get average of blob color value 
                          CvBlobs blobs);

