#include "main.hpp"

class ShapeDiscriptor
{
    private:
        
        //vector<ShapeProperty> shapeProperties;
        std::vector<cv::Mat> filterResults;
        bool doFilterInertia = false;
        bool doFilterCircularity = false;
        bool doFilterConvexity = false;
        //bool doFilterAvgColor = false;
        bool doFilterArea = false;
        float minInertia = 0.0f, maxInertia = 1.0f;
        float minCircularity = 0.0f, maxCircularity = 1.0f;
        float minConvexity = 0.0f, maxConvexity = 1.0f;
        //Point3f minAvgColor, maxAvgColor;
        int minArea = 0, maxArea = 1;

    public:

        void setInertia(float minInertia, float maxInertia);
        void setCircularityFilter(float minCircularity, float maxCircularity);
        void setConvexityFilter(float minConvexity, float maxConvexity);
        //void setAvgColorFilter(Point3f minColor, Point3f maxColor);
        void setAreaFilter(int minArea, int maxArea);
        const std::vector<cv::Mat>& discribeImages(const std::vector<cv::Mat> &images);
        bool discribeImage(const cv::Mat &image);
        //const std::vector<ShapeProperty>& applyFilter();
        
};

/*
typedef struct ShapeProperty{
    float inErtia;
    float circularity;
    float convexity;
    Point3f avgColor;
    int area;
}ShapeProperty;
*/
