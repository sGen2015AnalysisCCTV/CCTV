// This Code is for stationary frame.
// Never use it When the camera environment is moving

#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "ShapeDiscriptor.hpp" 
#include "main.hpp"

using namespace std;
using namespace cv;

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
	string p_head;
	string p_left_hand;
	string p_right_hand;
	string p_center;
	string hist;

	void push_head(cv::Point p) {
		p_head.clear();
		p_head += to_string(p.x);
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
	string giveMeJson() {
		string rtn;
		rtn += "{\"head\":{" + p_head + "},";
		rtn += "\"left\":{" + p_left_hand + "},";
		rtn += "\"right\":{" + p_right_hand + "},";
		rtn += "\"center\":{" + p_center + "},";
		rtn += "\"hist\":{" + hist + "}";
		rtn += "}";
		return rtn;
	}; 
};

int sendToServer(string post_data)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);
	/* videoId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* userId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* videoDangerPoint */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDangerPoint",
		CURLFORM_COPYCONTENTS, "70",
		CURLFORM_END);
	/* videoDate */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDate",
		CURLFORM_COPYCONTENTS, "2015.08.02/03:01",
		CURLFORM_END);
	/* Reason1 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason1",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason2 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason2",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason3 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason3",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason4 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason4",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason5 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason5",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* visitorFace */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "visitorFace",
		CURLFORM_FILE, "C:\\Users\\Public\\Pictures\\Sample Pictures\\Chrysanthemum.jpg",
		CURLFORM_END);
	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videofile",
		CURLFORM_FILE, "C:\\Users\\lec\\Desktop\\my\\p\\project_5_sgen_2015_summer_\\CCTV\\CCTV\\temp_origin.avi",
		CURLFORM_END);

	/* Fill in the filename field */
	/*
	curl_formadd(&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "filename",
	CURLFORM_COPYCONTENTS, "C:\\Users\\youjin\\hyj\\video\\test1.avi",
	CURLFORM_END);
	*/

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "submit",
		CURLFORM_COPYCONTENTS, "send",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initialize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if (curl) {
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, "http://210.125.96.109/upload.php");

		/*
		if ((argc == 2) && (!strcmp(argv[1], "noexpectheader")))
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		*/

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return 0;
};

int main() {
    ofstream				output;	
	cv::VideoCapture		cam;
	cv::VideoWriter			writer;
	cv::Mat					frame, thresh, fore;
	CvBlobs					blobs;
	std::vector<cv::Mat>	blobs_image, human_image;
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
	output.open(output_name, 'w');
	
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
				cout << post_data.giveMeJson() << endl;
				// output
				output << post_data.giveMeJson() << endl;
				// sending.... ? 
		}

        if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
                break; 
        } 
    } 


	// sending test 

	return 0;
}
