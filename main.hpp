#pragma once
// openCV
#include "opencv/cv.h"
#include "opencv/cvblob.h"

#ifdef __GNUC__
#include <opencv2/opencv.hpp>
#else
#include "opencv2/opencv.hpp"
#endif

// basic header
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <list> 
#include <time.h>
#include <thread> 
#include "CircularQueue.hpp"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

using namespace std;

// post class
// push each input functions.
// makeJson will return a string like below form.
/*
{ "head":{1,2},
"left":{1,2},
"right":{1,2},
"center":{1,2},
"hist":{1,2,3}
}
*/
// histogram is r g b oder.
class postClass {
public:
	std::string p_head;
	std::string p_left_hand;
	std::string p_right_hand;
	std::string p_center;
	std::string hist;
    std::string upperHand;

	void push_head(cv::Point p) {
		p_head.clear();
		p_head += std::to_string(p.x);
		p_head += ",";
		p_head += to_string(p.y);
	};
	void push_left_hand(cv::Point p) {
		p_left_hand.clear();
		p_left_hand += to_string(p.x);
		p_left_hand += ",";
		p_left_hand += to_string(p.y);
	};
	void push_right_hand(cv::Point p) {
		p_right_hand.clear();
		p_right_hand += to_string(p.x);
		p_right_hand += ",";
		p_right_hand += to_string(p.y);
	};
	void push_center(cv::Point p) {
		p_center.clear();
		p_center += to_string(p.x);
		p_center += ",";
		p_center += to_string(p.y);
	};
	void push_hist(int r, int g, int b) {
		hist.clear();
		hist += to_string(r);
		hist += ",";
		hist += to_string(g);
		hist += ",";
		hist += to_string(b);
	};
        void push_upperHand(bool isHarzardous)
        {
                upperHand.clear();
                upperHand += to_string(isHarzardous);
        }
	std::string giveMeJson() {
		std::string rtn;
		rtn += "{\"head\":[" + p_head + "],";
		rtn += "\"left\":[" + p_left_hand + "],";
		rtn += "\"right\":[" + p_right_hand + "],";
		rtn += "\"center\":[" + p_center + "],";
		rtn += "\"hist\":[" + hist + "]";
		//rtn += "\"upperHand\":[" + upperHand + "]";
		rtn += "}";
		return rtn;
	};
};
