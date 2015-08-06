#include "blobing.hpp"
Scalar getFrameDominantColor(Mat *frame, CvBlob* tblob)
{// function that returns dominant color in Blob Mat respectitively. 
// frame variable should be NOT GRAYSCALE.        
//
        Scalar color;
        int lx = tblob->minx, ly = tblob->miny;
        int rx = tblob->maxx, ry = tblob->maxy;
        int tr = 0, tg = 0, tb = 0;
        int r = 0,g = 0,b = 0;
        int pCount = 0;
        for(int y = ly; y < ry; y++)
        {
            for(int x = lx; x < rx; x++)
            {
                Vec3b tv = (*frame).at<Vec3b>(y, x);
                tb = tv.val[0]; tg = tv.val[1]; tr = tv.val[2]; 
                pCount++;
                r += tr; g += tg; b += tb;
            }
        }
        if(pCount == 0) return cv::Scalar(0);
        r /= pCount; g /= pCount; b /= pCount;
        color.val[0] = b; color.val[1] = g; color.val[2] = r;
        return color;
        
}

CvBlobs getBlobs(Mat *frame)
{// function that returns blob algorithm result with CvBlobs object.
    //frame     : the image that want to extract blobs
    //images    : the vector of image that store blob results.
    // !! frames should be gray scale image. if not, assertion will failed.
    CvBlobs blobs;
    IplImage *temp, *lImg = cvCreateImage(cvSize(VIDEO_WIDTH, VIDEO_HEIGHT), IPL_DEPTH_LABEL, 1);
        
    // convert cv::Mat to cv::IplImage
    temp= &IplImage(*frame); 
    // Do Blobing  
    unsigned int r = cvLabel(temp, lImg, blobs);
    cvFilterByArea(blobs, 5000, 100000); 
    cvReleaseImage(&lImg); 
    return blobs;
} 

void getBlobMat(Mat* frame, Mat*fore, CvBlobs blobs, vector<Mat>* images, Scalar &humanColor)
{// function that returns specific Mat area of CvBlob object respectively with vector object
    // assert((*frame).channels() != 1 && "frame is not grayscale");

    // to free memory 
    int max = 0;
    CvBlob* maxblob = NULL;
    for_each((*images).begin(), (*images).end(),[&](Mat m){m.release();}); (*images).clear();

    std::for_each(blobs.begin(), blobs.end(), 
            [&](pair<CvLabel, CvBlob*> blob){
                CvBlob* tblob = blob.second;
                Mat dst(Size(VIDEO_WIDTH, VIDEO_HEIGHT), CV_8UC1, 0);
                resize((*fore)(Rect(tblob->minx, tblob->miny, tblob->maxx-tblob->minx, tblob->maxy-tblob->miny)), 
                        dst, 
                        Size(tblob->maxx - tblob->minx, tblob->maxy - tblob->miny));
                (*images).push_back(dst);
				dst.release();
                if(tblob->area >= max)
                {
                    max = tblob->area;
                    maxblob = blob.second; 
                }

            });

    if(maxblob != NULL) 
        humanColor = getFrameDominantColor(frame, maxblob);



}

