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
	char*					window_name_main; 
	char*					video_name;
	char*					output_name;
	int						i, j; 
	cv::Point				p_head;
	cv::Point				p_left_hand;
	cv::Point				p_right_hand;
	cv::Point				p_center;
	int						hist_r, hist_g, hist_b;
	BackgroundSubtractorMOG2        bg;
	vector< vector<Point> >	contours; 
	cv::Mat					contours_image;
	postClass				post_data;
	bool					do_output;

	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 1;
	window_name_main = "asdf"; 
	video_name = "temp.avi";
	output_name = "data.txt"; 
	cam.open("temp_origin.avi"); //cam.open( 1 );
	cv::namedWindow( window_name_main );
	writer = cv::VideoWriter(video_name, CV_FOURCC('M', 'J', 'P', 'G'),
							15.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT),
							true);
	do_output = false;
	if( do_output ) output.open(output_name, 'w');
	
	// manual start 
    while( false ) {
        cam >> frame;
        if( waitKey( waitKey_delay ) == 32 ) 
            break;
        cv::imshow( window_name_main, frame );
    } 

	// main loop
    while( true ) {
        // get frame from cam
        cam >> frame;
            
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

		// filling
        contours_image = fore.clone();
		findContours(contours_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); 
		for (int i = 0; i < contours.size(); i++)
		{  
			if (contours[i].size() > 5000) break; // breaking memory exception
			double area0 = contourArea(contours[i]); 
			if (area0 < 2500) 
				drawContours(fore, contours, i, Scalar(0), CV_FILLED); 
			else 
				drawContours(fore, contours, i, Scalar(255), CV_FILLED); 
		}

		// blobing
		hist_r = hist_g = hist_b = 0; 

        blobs = getBlobs(&fore, &frame);
        getBlobMat(&frame,&fore, blobs, &blobs_image, hist_r, hist_g, hist_b);  

		if (blobs_image.size() > 0) {
			// skeleton
			p_head = p_left_hand = p_right_hand = cv::Point(0, 0);

			starSkeleton(blobs_image[0], blobs_image[0], p_head, p_left_hand, p_right_hand, p_center, 20);

			// show!
			cv::imshow(window_name_main, blobs_image[0]);

			// post data
				// make data
				post_data.push_head(p_head);
				post_data.push_left_hand(p_left_hand);
				post_data.push_right_hand(p_right_hand);
				post_data.push_center(p_center);
				post_data.push_hist(hist_r, hist_g, hist_b);
				// console print
				//cout << post_data.giveMeJson() << endl;
				// output
				if( do_output) output << post_data.giveMeJson() << endl;
				// sending.... ? 
		}

        if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
                break; 
        } 
    } 


	// sending test 

	system("pause");

	return 0;
}
