// This Code is for stationary frame.
// Never use it When the camera environment is moving

#include "blobing.hpp"
#include "starSkeleton.hpp"
#include "ShapeDiscriptor.hpp"
#include "main.hpp"
using namespace std;
using namespace cv;

int main() {
	
	cv::VideoCapture	cam;
	cv::VideoWriter		writer;
	cv::Mat			frame, thresh, fore;
        CvBlobs			blobs;
        std::vector<cv::Mat>		blobs_image, human_image;

	char				waitKey_exit;
	int					waitKey_delay;
	char*				window_name_main; 
	int					i, j; 
        bool				isRobbed = false; 
        BackgroundSubtractorMOG2        bg;

        clock_t frame_start, frame_end;
        clock_t ed_start, ed_end; 

        vector<thread> threads;
        int cores = thread::hardware_concurrency();
	// initialize
	waitKey_exit = 'q';
	waitKey_delay = 100;
	window_name_main = "규진이에게 샤오미 보조배터리를!!"; 
	
	cam.open( 0 );
	cv::namedWindow( window_name_main );
        cv::namedWindow("blobing");
	writer = cv::VideoWriter("temp.avi", CV_FOURCC('M', 'J', 'P', 'G'),
							15.0, Size(VIDEO_WIDTH, VIDEO_HEIGHT),
							true);


        ShapeDiscriptor sd;
        sd.setInertiaFilter(0.7f, 1.0f);
        sd.setCircularityFilter(0.4f, 1.0f);
        sd.setConvexityFilter(0.4f, 1.0f);
        for(int i = 0; i < 10; i++) cv::namedWindow(to_string(i));

        while( true ) {
                frame_start = clock(); 
                // get frame from cam
                cam >> frame;
                
                if(writer.isOpened() && isRobbed)
                {
                    //if 0 frame is written by writer, remove file.
                    //writer.write(frame);

                }
                // frame to be gray scale
                bg.operator()(frame, fore, 0);
                cv::threshold(fore,fore, 250, 255, 0);
                                
                for(int j = 0; j < cores; j++) 
                {
                    auto code = [&]()
                            {
                                for(int i = 0; i < 3; i++)
                                {
                                    erode(fore, fore, Mat());
                                    erode(fore, fore, Mat());
                                    dilate(fore, fore, Mat());
                                }
                           };
                    threads.push_back(thread(code));
                }
                for(thread& t: threads)
                {
                    if(t.joinable()) t.join();
                }

                
                blobs = getBlobs(&fore, &frame);
                getBlobMat(&fore, blobs, &blobs_image);
                sd.discribeImages(blobs_image);
                // blobing test
                
                if( true ) {
                        cout <<blobs_image.size() << endl;
                        for(int i = 0; i < blobs_image.size(); i++)
                        {
                            cv::imshow(to_string(i), blobs_image[i]);

                        }
                }
                
                imshow(window_name_main, frame);
                imshow("blobing", fore);
                //VideoWriter call distructor automatically.
                //frame release
                // delay
                if (cv::waitKey(waitKey_delay) == waitKey_exit ) {
                        break; 
                }

                frame_end = clock();
                printf("%f ms\n", (double)(frame_end-frame_start)/ CLOCKS_PER_SEC);
        }

	return 0;
}
