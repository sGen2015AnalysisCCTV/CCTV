// This Code is for stationary frame.
// Never use it When the camera environment is moving

#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "ShapeDiscriptor.hpp" 
#include "WebSocket.hpp"
#include "main.hpp"

using namespace std;
using namespace cv;  

int main() {  
    ofstream				output;	
	cv::VideoCapture		cam;
	cv::VideoWriter			writer;
	cv::Mat					frame, thresh, fore;
	CvBlobs					blobs;
	std::vector<cv::Mat>	blobs_image;
	bool					clicked;
	char					waitKey_exit;
	int						waitKey_delay;
	const char*					window_name_main; 
	const char*					video_name;
	const char*					output_name;
	const char*					video_webm_name;
	string						ffmpeg_convert_to_webm;
	int						i, j; 
    Human                                   humanCoord;
    Scalar                                  humanColor;
	int						hist_r, hist_g, hist_b;
	CircularQueue<cv::Mat>	video_queue(100); 
	BackgroundSubtractorMOG2        bg;
	vector< vector<Point> >	contours; 
	cv::Mat					contours_image;
	postClass				post_data;
	bool					do_output;
	bool					isDanger;
	bool					isUploading;
	bool					is_send_data;

	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 300;
	window_name_main = "asdf"; 
	video_name = "temp.avi";
	video_webm_name = "temp.webm";
	ffmpeg_convert_to_webm = "ffmpeg -i ";
	ffmpeg_convert_to_webm += video_name;
	ffmpeg_convert_to_webm += " -vcodec libvpx -y ";
	ffmpeg_convert_to_webm += video_webm_name;
	output_name = "data.txt"; 
	cam.open(0); 
	cv::namedWindow( window_name_main );
	do_output = false;
	isDanger = false;
	isUploading = false;
	is_send_data = false;
	if( do_output ) output.open(output_name, 'w'); 
	
	// manual start 
    while( true ) {
        cam >> frame;
        if( waitKey( waitKey_delay ) == 32 ) 
            break;
        cv::imshow( window_name_main, frame );
    } 

	// send thread create 
	thread sendToServerCaution_thread([&]() {
		string rtn;
		while (true) {
			if (!isUploading && is_send_data ) {
				rtn = sendToServerCaution(post_data.giveMeJson().c_str());
				if (rtn == "1")
					isDanger = true;
				is_send_data = false;
			}
		}
	});
	thread sendToServerVideo_thread([&]() {
		string rtn;
		while (true) {
			if (isDanger) {
				isUploading = true;
				// saveVideo
				writer = cv::VideoWriter(video_name, CV_FOURCC('M', 'P', '4', '2'), 3.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT), true);
				while (video_queue.size())
					writer << video_queue.dequeue();
				system(ffmpeg_convert_to_webm.c_str());
				rtn = sendToServerVideo(video_webm_name);
				if (rtn == "1")
					isUploading = isDanger = false;
			}
		}
	});


	// main loop
    while( true ) {
        // get frame from cam
        cam >> frame; 

		// enqueue
		if (!isDanger)
			video_queue.enqueue(frame);
            
		// background subtraction
		bg.operator()(frame, fore, 0);

        // frame to be gray scale
		cv::threshold(fore,fore, 250, 255, 0);

		// some erode and dilate
        erode(fore, fore, Mat());
        dilate(fore, fore, Mat());
        erode(fore, fore, Mat());
        dilate(fore, fore, Mat());
        dilate(fore, fore, Mat());
		
		//save test 
		/*
		int key = 0;
		imshow(window_name_main, frame);
		if (!isDanger)
			video_queue.enqueue(frame.clone());   
		key = cv::waitKey(waitKey_delay); 
		if (key == 'q') {
			break;
		}
		else if (key == 32) {
			if (!isDanger)
				isDanger = true;   
		}
		cout << video_queue.size() << endl;
		continue;
		*/
		 
		// filling 
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

		memset(&humanColor.val, 0, sizeof(humanColor.val));
		memset(&humanCoord, 0, sizeof(Human));

        blobs = getBlobs(&fore);
        getBlobMat(&frame,&fore, blobs, &blobs_image, humanColor);  

		if (blobs_image.size() > 0) { 
			// skeleton
			if (starSkeleton(blobs_image[0], blobs_image[0], humanCoord, 20)) {
				// show!
				cv::imshow(window_name_main, blobs_image[0]);

				// post data
					// make data
					post_data.push_head(humanCoord.head);
					post_data.push_left_hand(humanCoord.larm);
					post_data.push_right_hand(humanCoord.rarm);
					post_data.push_center(humanCoord.center);
					post_data.push_hist(humanColor.val[2], humanColor.val[1], humanColor.val[0]);
					//post_data.push_upperHand(isHarzardous(humanCoord));	
				// console print
					//cout << post_data.giveMeJson() << endl;
				// output
					if (do_output) output << post_data.giveMeJson() << endl;
				// sending
					is_send_data = true;
			}
		}

		if (cv::waitKey(waitKey_delay) == waitKey_exit)
			break; 
    } 
	 
	system("pause");

	return 0;
}