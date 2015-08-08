// This Code is for stationary frame.
// Never use it When the camera environment is moving

#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "ShapeDiscriptor.hpp" 
#include "WebSocket.hpp"
#include "main.hpp"
#include <Windows.h>
#include "subtraction.hpp"

using namespace std;
using namespace cv; 

bool isCamera(int number) {
	cv::VideoCapture cam;
	cam.open(number);
	return cam.isOpened();
};

int main() {
	ofstream				output;
	cv::VideoCapture		cam_alba, cam_thief;
	cv::VideoWriter			writer;
	cv::Mat					thresh, fore, convhull, frame_alba, frame_thief;
	CvBlobs					blobs;
	std::vector<cv::Mat>	blobs_image;
	bool					clicked;
	char					waitKey_exit;
	int						waitKey_delay;
	const char*				window_name_alba;
	const char*				window_name_thief;
	const char*					video_name;
	const char*					output_name;
	const char*					video_webm_name;
	string						ffmpeg_convert_to_webm;
	int						i, j;
	int						input;
	int						cam_number1, cam_number2;
	int						mode;
	Human                                   humanCoord;
	Scalar                                  humanColor;
	CircularQueue<cv::Mat>	video_queue(100);
	deque<postClass>		post_data_deque;
	string					post_json;
	BackgroundSubtractorMOG2        bg, bg2;
	vector< vector<Point> >	contours;
	cv::Mat					contours_image;
	postClass				post_data;
	bool					do_output;
	bool					isDanger;
	bool					isUploading;
	bool					is_send_data;
	bool					is_right_cam;
	VideoCapture cam1, cam2; 

	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 300;
	window_name_alba = "part timer camera";
	window_name_thief = "thief camera";
	video_name = "temp.avi";
	video_webm_name = "temp.webm";
	ffmpeg_convert_to_webm = "C:\\Users\\lec\\Desktop\\my\\p\\project_5_sgen_2015_summer_\\ffmpeg\\bin\\ffmpeg.exe -i ";
	ffmpeg_convert_to_webm += video_name;
	ffmpeg_convert_to_webm += " -vcodec libvpx -y ";
	ffmpeg_convert_to_webm += video_webm_name;
	output_name = "data.txt";
	do_output = false;
	isDanger = false;
	isUploading = false;
	is_send_data = false;
	is_right_cam = true;
	if (do_output) output.open(output_name, 'w');

	// initialize with user input
MAIN:
	system("cls");
	cout << "-----------------------------------------------" << endl;
	cout << "CCTV Analysizer" << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "1. Background-remove" << endl;
	cout << "2. Star-Skeletonization" << endl;
	cout << "3. Send json" << endl;
	cout << "4. Send video" << endl;
	cout << "5. Start" << endl;
	cin >> mode;

	system("cls");
	switch (mode) {
	case 1: case 2: case 3:
		cout << "Input camera number" << endl;
		cin >> cam_number1;
		if (!isCamera(cam_number1)) {
			cout << "Cannot find camera at " << cam_number1 << endl;
			goto MAIN;
		}
		break;
	case 4: case 5:
		cout << "Input first camera number" << endl;
		cin >> cam_number1;
		if (!isCamera(cam_number1)) {
			cout << "Cannot find camera at " << cam_number1 << endl;
			goto MAIN;
		}
		cout << "Input second camera number" << endl;
		cin >> cam_number2;
		if (!isCamera(cam_number2)) {
			cout << "Cannot find camera at " << cam_number2 << endl;
			goto MAIN;
		}
		// validate it's right cam
		/*
		cam1.open(cam_number1);
		cam2.open(cam_number2);
		thread isRightCam([&]() {
			while (is_right_cam) {
				cam1 >> frame_alba;
				cam2 >> frame_thief;
				cv::imshow(window_name_alba, frame_alba);
				cv::imshow(window_name_thief, frame_thief);
				waitKey(1);
			}
			cam1.release();
			cam2.release();
		});
		*/
		system("cls");
		cout << "are cameras capturing appropriate object?" << endl;
		cout << "yes is 1. no is 2" << endl;
		cin >> input;
		if (input == 2) {
			cam_number1 = cam_number1 ^ cam_number2;
			cam_number2 = cam_number1 ^ cam_number2;
			cam_number1 = cam_number1 ^ cam_number2;
		}
		is_right_cam = false;
		break;
	}

	// send thread create 
	thread sendToServerCaution_thread([&]() {
		string rtn;
		while (mode >= 3 && !isDanger) {
			if (!isUploading && is_send_data) {
				rtn = sendToServerCaution(post_json.c_str());
				if (rtn[0] == '1' && !isUploading)
					isDanger = true;
				if (mode == 3)
					cout << "return : " << rtn << endl;
				is_send_data = false;
			}
		}
	});
	thread sendToServerVideo_thread([&]() {
		string rtn;
		while (mode >= 4) {
			if (isDanger) {
				isUploading = true;
				// saveVideo
				writer = cv::VideoWriter(video_name, CV_FOURCC('M', 'P', '4', '2'), 3.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT), true);
				while (video_queue.size())
					writer << video_queue.dequeue();
				system(ffmpeg_convert_to_webm.c_str());
				rtn = sendToServerVideo(video_webm_name);
				if (rtn[0] == '1') {
					Sleep(10000); isUploading = isDanger = false;
				}
			}
		}
	});

	// cam open
	if (mode >= 4)
		cam_thief.open(cam_number2);
	cam_alba.open(cam_number1);

	// preprocess of background-remove
	while (true) {
		cam_alba >> frame_alba;
		if (waitKey(waitKey_delay) == 32)
			break;
		cv::imshow(window_name_alba, frame_alba);
	}

	// main loop
	while( true ) {
		cam_alba >> frame_alba;
		if (mode >= 4) {
			cam_thief >> frame_thief;
			video_queue.enqueue(frame_thief.clone());
			cv::imshow(window_name_thief, frame_thief);
		}

		// background-remove
		cv::Mat				contours_image;
		bg.operator()(frame_alba, fore, -1);
		
		cv::threshold(fore, fore, 250, 255, 0);
		//for (int i = 0; i < 4; i++) erode(fore, fore, Mat());
		for (int i = 0; i < 7; i++) dilate(fore, fore, Mat());
		imshow("bgsub", fore);
		// filling
		contours_image = fore.clone();
		findContours(contours_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
				if (contours[i][j].x < 0 || contours[i][j].y < 0)
					goto EXITS;
			double area0 = contourArea(contours[i]);
			if (area0 < 20000)
				drawContours(fore, contours, i, Scalar(0), CV_FILLED);
			else
			{
				drawContours(fore, contours, i, Scalar(255), CV_FILLED);

			}
		}
		if (contours.size() < 100000 && contours.size() > 4)
		{
			vector<vector<Point> > hull(contours.size());
			for (int i = 0; i < 10; i++) dilate(fore, fore, Mat());
			for (int i = 0; i < contours.size(); i++)
			{
				for (int j = 0; j < contours[i].size(); j++)
					if (contours[i][j].x < 0 || contours[i][j].y < 0)
						goto EXITS;
				//if (contours[i].size() > 5000) break;

				approxPolyDP(Mat(contours[i]), hull[i], 10, true);
				drawContours(fore, hull, i, Scalar(255, 0, 255), CV_FILLED);
			}
		}
		for (int i = 0; i < 4; i++) erode(fore, fore, Mat());
		imshow("fore", fore);
	EXITS:
		//subtraction(bg, bg2, frame_alba, fore, thresh);
		/*bg.operator()(frame_alba, fore, 0);
		cv::threshold(fore, fore, 250, 255, 0);
		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
		dilate(fore, fore, Mat());
		contours_image = fore.clone();
		findContours(contours_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
				if (contours[i][j].x < 0 || contours[i][j].y < 0)
					goto EXIT_FILLING;

			double area0 = contourArea(contours[i]);
			if (area0 < 2500)
				drawContours(fore, contours, i, Scalar(0), CV_FILLED);
			else
				drawContours(fore, contours, i, Scalar(255), CV_FILLED);
		}
	EXIT_FILLING:
	*/
		memset(&humanColor.val, 0, sizeof(humanColor.val));
		memset(&humanCoord, 0, sizeof(Human));

		blobs = getBlobs(&fore);
		getBlobMat(&frame_alba, &fore, blobs, &blobs_image, humanColor);
		
		if (!blobs_image.empty()) {
			if (mode > 1) {
				if (starSkeleton(blobs_image[0], blobs_image[0], humanCoord, 20)) {
					if (mode >= 3) {
						post_data.push_head(humanCoord.head);
						post_data.push_left_hand(humanCoord.larm);
						post_data.push_right_hand(humanCoord.rarm);
						post_data.push_center(humanCoord.center);
						post_data.push_hist(humanColor.val[2], humanColor.val[1], humanColor.val[0]);

						if (post_data_deque.size() > 3)
							post_data_deque.pop_front();
						while (post_data_deque.size() < 3)
							post_data_deque.push_back(post_data);
						post_data_deque.push_back(post_data);

						post_json.clear();
						for (int a = 0; a < 3; a++) {
							post_json += post_data_deque[a].giveMeJson() + " ";
						}
						if (mode == 3)
							cout << post_data.giveMeJson() << endl;
						// sending
						is_send_data = true;
					}
				}
			}
			cv::imshow(window_name_alba, blobs_image[0]);
		}

		if (waitKey(waitKey_delay) == waitKey_exit) break;
			
	}


	system("pause");

	return 0;
}