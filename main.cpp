#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "main.hpp"
using namespace std;
using namespace cv;

int main() {
	
	cv::VideoCapture	cam;
	cv::Mat			frame;
        CvBlobs                 blobs;
	vector<cv::Mat>		blobs_image;

	char			waitKey_exit;
	int			waitKey_delay;
	string			window_name_main;

	int			i, j;

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
                blobs = getBlobs(&frame);
                getBlobMat(&frame, blobs, &blobs_image);
                // blobing test
                if( false ) {
                        for( i=0; i<blobs.size(); i++ ) {
                                cv::namedWindow( std::to_string((long double)i) );
                                cv::imshow( std::to_string((long double)i), blobs_image[i] );
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
