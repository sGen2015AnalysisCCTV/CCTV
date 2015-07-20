#include "blobing.hpp"
#include "starSkeleton.hpp"

using namespace std;
using namespace cv;

int main() {
	
	cv::VideoCapture	cam;
	cv::Mat				frame;
	vector<cv::Mat>		blobs;

	int					waitKey_delay;
	string				window_name_main;

	int					i, j;

	// initialize variable
		waitKey_delay = 100;
		window_name_main = "규진이에게 샤오미 보조배터리를!!";

	// initialize
		cam.open( 0 );
		cv::namedWindow( window_name_main );

	// main loop
		while( true ) {
			
			// get frame from cam
				cam >> frame;

			// blobing
				getblobs( frame, blobs );
			
			// blobing test


			// delay
				cv::waitKey( waitKey_delay );
		}

	// end
		return 0;
}