#include "ShapeDiscriptor.hpp"

bool ShapeDiscriptor::discribeImage(cv::Mat &image)
{
    std::vector< std::vector<cv::Point> > contours;
    cv::Mat timage = image.clone();
    printf("rows : %d\n", image.rows);
    findContours(timage, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    for(int contourIdx = 0; contourIdx < contours.size(); contourIdx++)
    {

        cv::Moments moms = moments(cv::Mat(contours[contourIdx]));
        if(doFilterArea)
        {
            double area = moms.m00;
            if (area < minArea || area > maxArea)
            //drawContours(image, contours, contourIdx, cv::Scalar(0,0,0), CV_FILLED);
            continue;
        }
        
        if(doFilterCircularity)
        {
          double area = moms.m00;
          double perimeter = cv::arcLength(cv::Mat(contours[contourIdx]), true);
          double ratio = 4 * CV_PI * area / (perimeter * perimeter);
          if (ratio < minCircularity)
            //drawContours(image, contours, contourIdx, cv::Scalar(0,0,0), CV_FILLED);
            continue;
        }

        if(doFilterInertia)
        {
          double denominator = sqrt(pow(2 * moms.mu11, 2) + pow(moms.mu20 - moms.mu02, 2));
          const double eps = 1e-2;
          double ratio;
          if (denominator > eps)
          {
            double cosmin = (moms.mu20 - moms.mu02) / denominator;
            double sinmin = 2 * moms.mu11 / denominator;
            double cosmax = -cosmin;
            double sinmax = -sinmin;

            double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
            double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
            ratio = imin / imax;
          }
          else
          {
            ratio = 1;
          }
          //p.inErtia = ratio;
          if (ratio < minInertia)
             //drawContours(image, contours, contourIdx, cv::Scalar(0,0,0), CV_FILLED);
             
            continue;
        }

        if(doFilterConvexity)
        {
          cv::vector < cv::Point > hull;
          convexHull(cv::Mat(contours[contourIdx]), hull);
          double area = cv::contourArea(cv::Mat(contours[contourIdx]));
          double hullArea = cv::contourArea(cv::Mat(hull));
          double ratio = area / hullArea;
          //p.convexity = ratio;
          if (ratio < minConvexity)
            //drawContours(image, contours, contourIdx, cv::Scalar(0,0,0), CV_FILLED);

            continue;
        }

        timage.release();
        return true;
    }
    timage.release();
    return false;
} 

void ShapeDiscriptor::discribeImages(std::vector<cv::Mat> &images)
{
    for(std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end();)
    {
        if(!discribeImage(*iter)){
            images.erase(iter);

        }else
        {
            iter++;

        }

    }
}

void ShapeDiscriptor::setInertiaFilter(float minInertia, float maxInertia)
{
    assert(minInertia >= 0.0f && maxInertia <= 1.0f);
    doFilterInertia = true;
    this->minInertia = minInertia;
    this->maxInertia = maxInertia;
}

void ShapeDiscriptor::setCircularityFilter(float minCicularity, float maxCircularity)
{
    assert(minCircularity >= 0.0f || maxCircularity <= 1.0f);
    doFilterCircularity = true;
    this->minCircularity = minCircularity;
    this->maxCircularity = maxCircularity;
}

void ShapeDiscriptor::setConvexityFilter(float minConvexity, float maxConvexity)
{
    assert(minConvexity >= 0.0f || maxConvexity < 1.0f);
    doFilterConvexity = true;
    this->minConvexity = minConvexity;
    this->maxConvexity = maxConvexity;
}
/*
void ShapeDiscriptor::setAvgColorFilter(Point3f minAvgColor, Point3f maxAvgColor)
{
    doFilterByColor = true;
    this->minAvgColor = minAvgColor;
    this->maxAvgColor = maxAvgColor;
}
*/
void ShapeDiscriptor::setAreaFilter(int minArea, int maxArea)
{
    doFilterArea = true;
    this->minArea = minArea;
    this->maxArea = maxArea;
}


