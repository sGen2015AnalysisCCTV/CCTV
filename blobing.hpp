#pragma once
#include "main.hpp"

using namespace std;
using namespace cv;
using namespace cvb;

CvBlobs getBlobs(Mat *frame, Mat* oframe=nullptr);           // get frame blob results

void getBlobMat(Mat *frame, 
                Mat* fore,
                CvBlobs blobs,         // get Mat(VIDEO_WIDTH * VIDEO_HEIGHT)
                vector<Mat>* images,
                FILE* output = 0);   // blob in blobs respectively.

void getFrameDominantColor(Mat *frame,   // get average of blob color value 
                          CvBlobs blobs, 
                          vector<Scalar>* blobs_color, FILE* output = 0);

