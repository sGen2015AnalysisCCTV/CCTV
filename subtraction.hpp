#pragma once

#include "main.hpp"
using namespace cv;

void subtraction(BackgroundSubtractorMOG2 &bg, BackgroundSubtractorMOG2 &bg2, Mat& frame, Mat& fore, Mat& convhull)
{
	vector< vector<Point> >	        contours;
	cv::Mat				contours_image;
	Mat fore2;
	bg.operator()(frame, fore, 0);
	bg2.operator()(frame, fore2, -0.1);
	//threshold(fore, fore, )
	imshow("bgoperator", fore);
	imshow("bgoperator2", fore2);
	for (int i = 0; i < 6; i++) dilate(fore, fore, Mat());
	// filling
	contours_image = fore.clone();
	findContours(contours_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	vector<vector<Point> > hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{

		double area0 = contourArea(contours[i]);
		if (area0 < 20000)
			drawContours(fore, contours, i, Scalar(0), CV_FILLED);
		else
			drawContours(fore, contours, i, Scalar(255), CV_FILLED);
	}


	for (int i = 0; i < 5; i++) dilate(fore, fore, Mat());
	for (int i = 0; i < contours.size(); i++)
	{
		printf("%d\n", contours[i].size());
		convhull = fore.clone();
		approxPolyDP(Mat(contours[i]), hull[i], 10, true);
		drawContours(convhull, hull, i, Scalar(255, 0, 255), CV_FILLED);
	}
}
