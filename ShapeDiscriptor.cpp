#include "ShapeDiscriptor.hpp"

bool ShapeDiscriptor::discribeImage(const cv::Mat &image)
{
    cv::Mat tmpImage;
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    //ShapeProperty p;
    
    findContours(contours, image, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

    for(int contourIdx = 0; contourIdx < contours.size(); contourIdx++)
    {
        cv::Moments moms = moments(cv::Mat(contours[contourIdx]));
        if(doFilterArea)
        {
          double area = moms.m00;
          //p.area = area;
          if (area < minArea || area > maxArea)
            continue;
        }
        
        if(doFilterCircularity)
        {
          double area = moms.m00;
          double perimeter = cv::arcLength(cv::Mat(contours[contourIdx]), true);
          double ratio = 4 * CV_PI * area / (perimeter * perimeter);
          //p.circularity = ratio;
          if (ratio < minCircularity)
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
            continue;
        }

        return true;
    }
    return false;
} 

const std::vector<cv::Mat>& ShapeDiscriptor::discribeImages(const std::vector<cv::Mat> &images)
{
    std::for_each(images.begin(), images.end(),
           [&](cv::Mat image){
                if(discribeImage(image))
                    filterResults.push_back(image);
           });
    return filterResults;
}

void ShapeDiscriptor::setInertia(float minInertia, float maxInertia)
{
    assert(minInertia < 0 || maxInertia > 1);
    doFilterInertia = true;
    this->minInertia = minInertia;
    this->maxInertia = maxInertia;
}

void ShapeDiscriptor::setCircularityFilter(float minCicularity, float maxCircularity)
{
    assert(minCircularity < 0 || maxCircularity > 1);
    doFilterCircularity = true;
    this->minCircularity = minCircularity;
    this->maxCircularity = maxCircularity;
}

void ShapeDiscriptor::setConvexityFilter(float minConvexity, float maxConvexity)
{
    assert(minConvexity < 0 || maxConvexity > 1);
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


