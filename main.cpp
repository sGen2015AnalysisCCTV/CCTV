#include "main.hpp"

using namespace cv;
using namespace std;

Mat image;
int vmin = 10, vmax = 256, smin = 30;


int main()
{
    // C++ 방식

    //Backgorund Subtraction
    vector< vector<Point> > contours;
    BackgroundSubtractorMOG2 bg;
    Mat frame, gframe, tframe, fore;

    int E_OPERATION_SIZE = 4;
    Mat erosion = getStructuringElement(
            MORPH_ELLIPSE, 
            Size(2 * E_OPERATION_SIZE + 1, 2* E_OPERATION_SIZE + 1), 
            Point(E_OPERATION_SIZE, E_OPERATION_SIZE)
            );
    int D_OPERATION_SIZE = 4;
    Mat dilation = getStructuringElement(
            MORPH_ELLIPSE, 
            Size(2 * D_OPERATION_SIZE + 1, 2* D_OPERATION_SIZE + 1), 
            Point(D_OPERATION_SIZE, D_OPERATION_SIZE)
            );
    //Cam Initialize
    VideoCapture capture;
    capture.open(0);
      
    namedWindow("original", 0);
    namedWindow("fore", 1);
    namedWindow("meanshift",2);

    createTrackbar("vmin", "meanshift", &vmin, 256, 0);
    createTrackbar("vmax", "meanshift", &vmax, 256, 0);
    createTrackbar("smin", "meanshift", &smin, 256, 0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    if(!capture.isOpened())
    {
        cerr << "could not opened"<< endl;
        return 0;
    }
    while(true)
    {
        try
        {
            capture >> frame;
            bg.operator() (frame, fore, 0);
            threshold(fore, fore, 250,255,0);

            for(int i =0 ; i < 3; i++)erode(fore, fore, Mat());
            //for(int i = 0; i < 1; i++)dilate(fore, fore, Mat());

            ////////////// masking with background /////////////////////////
            int cols = fore.cols;
            int rows = fore.rows;
            for(int i = 0; i < rows; i++)
            {
                unsigned char* op = frame.ptr<unsigned char>(i); //original
                unsigned char* ap = fore.ptr<unsigned char>(i); //after
                for(int j = 0; j < cols; j++)
                {
                    if(ap[j] == 0) 
                    {
                        op[j*3] = 0;    //B
                        op[j*3+1] = 0;  //G
                        op[j*3+2] = 0;  //R
                    }

                }
    
            }
            //////////////////////////////////////////////////////////////
            

			// skeletonization
			
				Mat skel( fore.size(), CV_8UC1, cv::Scalar(0) );
				Mat tmp( fore.size(), CV_8UC1);
				Mat element = getStructuringElement( cv::MORPH_CROSS, cv::Size(3, 3) );
				bool done;
				do
				{
					cv::morphologyEx(fore, tmp, cv::MORPH_OPEN, element);
					cv::bitwise_not(tmp, tmp);
					cv::bitwise_and(fore, tmp, tmp);
					cv::bitwise_or(skel, tmp, skel);
					cv::erode(fore, fore, element);
 
					double max;
					cv::minMaxLoc(fore, 0, &max);
					done = (max == 0);
				} while (!done);
				
			// end skeletonization
            
            imshow("original", frame);
            imshow("fore", skel);
            imshow("meanshift", frame);   
        }
        catch(Exception& e)
        {
            cerr << "exception : " << e.err << endl;
        }
   
        if(waitKey(30) >= 0)break;
    }

}
