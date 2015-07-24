#include "main.hpp"

using namespace std;
using namespace cv;
using namespace cvb;

CvBlobs getBlobs(Mat *frame);           // get frame blob results

void getBlobMat(Mat *frame,
                CvBlobs blobs,         // get Mat(VIDEO_WIDTH * VIDEO_HEIGHT)
                vector<Mat>* images);   // blob in blobs respectively.

void getBlobDominantColor(Mat *frame,   // get average of blob color value 
                          CvBlobs blobs, 
                          vector<Scalar>* blobs_color);

