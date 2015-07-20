#include "blobing.hpp"
#include "starSkeleton.hpp"

using namespace std;
using namespace cv;

int main() {
	
	cv::VideoCapture	cam;
	cv::Mat				frame;
	vector<cv::Mat>		blobs;

	char				waitKey_exit;
	int					waitKey_delay;
	string				window_name_main;

	int					i, j;

	// initialize variable
		waitKey_exit = 'q';
		waitKey_delay = 100;
		window_name_main = "규진이에게 샤오미 보조배터리를!!";

	// initialize
		cam.open( 0 );
		cv::namedWindow( window_name_main );

	// main loop
		while( true ) {
			
			// get frame from cam
				cam >> frame;

			// frame to be gray scale
				cv::cvtColor( frame, frame, CV_BGR2GRAY );

			// blobing
				getblobs( &frame, &blobs );
			
			// blobing test
				if( false ) {
					for( i=0; i<blobs.size(); i++ ) {
						cv::namedWindow( to_string((long double)i) );
						cv::imshow( to_string((long double)i), blobs[i] );
					}
				} 

			// delay
				if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
					break; 
				}
		}

	// end
		return 0;
}