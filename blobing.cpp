#include "blobing.hpp"

CvBlobs getBlobs(Mat *frame)
{// function that returns blob algorithm result with CvBlobs object.
    //frame     : the image that want to extract blobs
    //images    : the vector of image that store blob results.
    // !! frames should be gray scale image. if not, assertion will failed.
    CvBlobs blobs;
    IplImage *temp;
    IplImage *lImg = cvCreateImage(cvSize(VIDEO_WIDTH, VIDEO_HEIGHT), IPL_DEPTH_LABEL, 1);
    
    //to free memory 
    for(int i = 0; i < (*images).size(); i++)
    {
        (*images).at(i).release();
    }

    (*images).clear();
    // convert cv::Mat to cv::IplImage
    temp= &IplImage(*frame);

    // Do Blobing  
    unsigned int r = cvLabel(temp, lImg, blobs);
    cvFilterByArea(blobs, 400, 10000000); 

    return blobs;
} 

void getBlobMat(CvBlobs blobs, vector<Mat>* images)
{// function that returns specific Mat area of CvBlob object respectively with vector object
    
    CvBlob *blob;

    for(CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
    {
        blob = (*it).second;
//      printf("%d %d %d %d\n", blob->minx, blob->miny, blob->maxx, blob->maxy);
//        
//      it references the original image. but it can uses limit by area.

        Mat roi = (*frame)(Rect(blob->minx, blob->miny, blob->maxx - blob->minx, blob->maxy - blob->miny));
        Mat dst(Size(VIDEO_WIDTH, VIDEO_HEIGHT), CV_8UC1 ,0);

        resize(roi, dst, Size(VIDEO_WIDTH, VIDEO_HEIGHT));
        (*images).push_back(dst); 
    }
}

void getBlobDominantColor(Mat *frame, CvBlobs blobs, vector<Scalar>* blobs_color)
{// function that returns dominant color in Blob Mat respectively. 
// frame variable should be NOT GRAYSCALE.

    CvBlob *blob;
    int pCount = 0;

    for(CvBlobs::const_iterator it=blobs.begin(); it != blobs.end(); ++it)
    {
        
        blob = (*it).second;  
        int tr = 0, tg = 0, tb = 0;
        int r = 0,g = 0,b = 0;
        Vec3b tv;

        for(int y = blob->miny; y < blob->maxy; y++)
        {
            for(int x = blob->minx; x < blob->maxx; x++)
            {
                tv = (*frame).at<Vec3b>(y, x)
                tb = tv.val[0]; tg = tv.val[1]; tr = tv.val[2];

                if(tr || tg || tb == 0) continue;
                else
                {
                    pCount++;
                    r += tr; g += tg; b += tb;
                }

            }
        }

        r /= pCount; g /= pCount; b /= pCount;
        (*blobs_color).push_Back(Scalar(r, g, b));
    }
}
