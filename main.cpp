// This Code is for stationary frame.
// Never use it When the camera environment is moving

#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "ShapeDiscriptor.hpp"
#include "RegressionAnalysis.hpp"
#include "main.hpp"
using namespace std;
using namespace cv;

int main() {
        FILE*                   output;	
	cv::VideoCapture	cam;
	cv::VideoWriter		writer;
	cv::Mat			frame, thresh, fore;
        CvBlobs			blobs;
        std::vector<cv::Mat>		blobs_image, human_image;
        bool                            clicked;
	char				waitKey_exit;
	int				waitKey_delay;
	char*				window_name_main; 
	int				i, j; 
        BackgroundSubtractorMOG2        bg;
	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 100;
	window_name_main = "규진이에게 샤오미 보조배터리를!!"; 
	
	cam.open( 1 );
	cv::namedWindow( window_name_main );
        cv::namedWindow("blobing");
	writer = cv::VideoWriter("temp.avi", CV_FOURCC('M', 'J', 'P', 'G'),
							15.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT),
							true);
        output = fopen("data.txt", "w");

        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;
        cv::Mat contours_image;
        while( true ) {
            cam >> frame;
            if( waitKey( waitKey_delay ) == 32 ) 
                break;
            imshow( window_name_main, frame );
        }
        while( true ) {
                // get frame from cam
                cam >> frame;
               
                // frame to be gray scale
                bg.operator()(frame, fore, 0);                
                cv::threshold(fore,fore, 250, 255, 0);
                erode(fore, fore, Mat());
                dilate(fore, fore, Mat());
                erode(fore, fore, Mat());
                dilate(fore, fore, Mat());
                dilate(fore, fore, Mat());

                contours_image = fore.clone();
                findContours(contours_image, contours,hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
                for(int i = 0; i < contours.size(); i++)
                {
                    double area0 = contourArea(contours[i]);
                    if(area0 < 2500)
                    {
                        drawContours(fore, contours, i, Scalar(0,0,0), CV_FILLED);  
                    }
                    else
                    {
                        drawContours(fore, contours, i, Scalar(255,255,255), CV_FILLED);

                    }

                }
                               
                blobs = getBlobs(&fore, &frame);
                getBlobMat(&frame,&fore, blobs, &blobs_image, output);
                cvtColor(fore, thresh, CV_GRAY2RGB);
                writer.write(thresh);

                for(int i = 0; i < blobs_image.size(); i++)
                {
                    if(i == 1) break;
                    starSkeleton(blobs_image[i], blobs_image[i], 3, 50, output);

                } 
                if( true ) {
                        cout << blobs_image.size() << endl;
                        for(int i = 0; i < blobs_image.size(); i++)
                        {
                            if(i == 1) break;
                            cv::imshow(to_string(i), blobs_image[i]);

                        }
                }
                 
                imshow(window_name_main, frame);
                imshow("blobing", fore);
                if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
                        break; 
                }

                fflush(output);
                }
        fclose(output);
	return 0;
}
