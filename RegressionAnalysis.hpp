#pragma once
#include "main.hpp"
#define RA_LINEAR 0
#define RA_MULTIPLE 1

int* linearRegression(int* coefficients, vector< vector<cv::Point> > points)
{
    //find y = ax + b form linear equation of given data set.
    // a = sum of x and y covarience / sum of x varience;
    // b = average of y coordinates - average of x coordinates * a value
    int pnum = 0;   //total points number
    int sx = 0, sy = 0; //sum of x, y cordinates;
    double ax, ay; //average x, y coordinates;
    double vx, vy; //sum of x, y varience respectitively
    double vxy;    //sum of x, y covarience
    

    for(int i = 0; i < points.size(); i++)
    {
        vector<cv::Point> point = points.at(i);
        pnum += point.size();
        for(int j = 0; j < points.size(); j++)
        {
            Point p = point.at(i);
            sx += p.x;
            sy += p.y;
        }
    }

    //get average;
    ax = sx / pnum;
    ay = sy / pnum;

    for(int i = 0; i < points.size(); i++)
    {
        vector<cv::Point> point = points.at(i);
        for(int j = 0; j < points.size(); j++)
        {
            Point p = point.at(i);
            vx += (p.x - ax) * (p.x - ax);
            vy += (p.y - ay) * (p.y - ay);
            vxy += (p.x - ax) * (p.y - ay);
        }
    }
    coefficients[1] = vxy / vx;
    coefficients[0] = ay - ax * coefficients[1];
    return coefficients;
}

int* multiVariableRegression()
{

}
