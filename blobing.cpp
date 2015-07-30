#include "blobing.hpp"

CvBlobs getBlobs(Mat *frame, Mat* oframe)
{// function that returns blob algorithm result with CvBlobs object.
    //frame     : the image that want to extract blobs
    //images    : the vector of image that store blob results.
    // !! frames should be gray scale image. if not, assertion will failed.
    CvBlobs blobs;
    IplImage *temp, *otemp;
    IplImage *lImg = cvCreateImage(cvSize(VIDEO_WIDTH, VIDEO_HEIGHT), IPL_DEPTH_LABEL, 1);
    
    // convert cv::Mat to cv::IplImage
    temp= &IplImage(*frame);
    otemp = &IplImage(*oframe);
    // Do Blobing  
    unsigned int r = cvLabel(temp, lImg, blobs);
    /*
    for_each(blobs.begin(), blobs.end(),
            [&](pair<CvLabel, CvBlob*> blob){
                CvBlob* tblob = blob.second;
                

            });*/


    cvFilterByArea(blobs, 5000, 100000);

    
    cvReleaseImage(&lImg);
    /*
    if(oframe != nullptr)
    {
        cvRenderBlobs(lImg, blobs, otemp, otemp, CV_BLOB_RENDER_BOUNDING_BOX); 
    }
    */
    return blobs;
} 

void getBlobMat(Mat* frame, CvBlobs blobs, vector<Mat>* images)
{// function that returns specific Mat area of CvBlob object respectively with vector object
    // assert((*frame).channels() != 1 && "frame is not grayscale");

    // to free memory 
    for_each((*images).begin(), (*images).end(),[&](Mat m){m.release();}); (*images).clear();

    std::for_each(blobs.begin(), blobs.end(), 
            [&](pair<CvLabel, CvBlob*> blob){
                CvBlob* tblob = blob.second;
                Mat dst(Size(VIDEO_WIDTH, VIDEO_HEIGHT), CV_8UC1, 0);
                resize((*frame)(Rect(tblob->minx, tblob->miny, tblob->maxx-tblob->minx, tblob->maxy-tblob->miny)), 
                        dst, 
                        Size(VIDEO_WIDTH, VIDEO_HEIGHT));
                (*images).push_back(dst);
            });

}

void getBlobDominantColor(Mat *frame, CvBlobs blobs, vector<Scalar>* blobs_color)
{// function that returns dominant color in Blob Mat respectively. 
// frame variable should be NOT GRAYSCALE.

    for_each(blobs.begin(), blobs.end(), 
            [&](pair<CvLabel, CvBlob*> blob){
                CvBlob* tblob = blob.second;
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

                        if(tr || tg || tb == 0) continue;
                        else
                        {
                            pCount++;
                            r += tr; g += tg; b += tb;
                        }
                    }
                }
                r /= pCount; g /= pCount; b /= pCount;
                (*blobs_color).push_back(Scalar(r, g, b));
            });

}

