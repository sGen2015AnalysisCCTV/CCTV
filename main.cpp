#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "main.hpp"
using namespace std;
using namespace cv;

int main() {
	
	cv::VideoCapture	cam;
	cv::VideoWriter		writer;
	cv::Mat			frame, thresh, fore;
        CvBlobs			blobs;
	vector<cv::Mat>		blobs_image;

	char				waitKey_exit;
	int					waitKey_delay;
	char*				window_name_main; 
	int					i, j; 
        bool				isRobbed = false; 
        BackgroundSubtractorMOG2        bg;
	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 100;
	window_name_main = "규진이에게 샤오미 보조배터리를!!"; 
	
	cam.open( 0 );
	cv::namedWindow( window_name_main );
	writer = cv::VideoWriter("temp.avi", CV_FOURCC('M', 'J', 'P', 'G'),
							15.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT),
							true);

	// main loop
        while( true ) {
                
                // get frame from cam
                cam >> frame;
                
                if(writer.isOpened() && isRobbed)
                {
                    //if 0 frame is written by writer, remove file.
                    writer.write(frame);

                }
                // frame to be gray scale
                bg.operator()(frame, fore, 0);
                cv::threshold(fore,fore, 250, 255, 0);
                for(int i = 0; i < 5; i++)
                {
                    erode(fore, fore, Mat());
                    dilate(fore, fore, Mat());
                }
                // blobing
                blobs = getBlobs(&fore);
                getBlobMat(&frame, blobs, &blobs_image);
                // blobing test
                if( false ) {
                        
                } 
               
                //VideoWriter call distructor automatically.
                //frame release
                frame.release();
                // delay
                if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
                        break; 
                }
        }

	// end
	return 0;
}
