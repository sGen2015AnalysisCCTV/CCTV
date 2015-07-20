#include "main.hpp"

using namespace std;
using namespace cv;
using namespace cvb; 

void getblobs( Mat &frame, vector<Mat> &images)
{
	//frame     : the image that want to extract blobs
	//images    : the vector of image that store blob results.
	// !! frames should be gray scale image. if not, assertion will failed. 

	CvBlobs blobs;
	IplImage *temp;
	IplImage *lImg = cvCreateImage(cvSize(320, 240), IPL_DEPTH_LABEL, 1);
 
	images.clear();
	// convert cv::Mat to cv::IplImage
	temp= &IplImage(frame);
	
	// Do Blobing  
	unsigned int r = cvLabel(temp, lImg, blobs);
	cvFilterByArea(blobs, (unsigned int)400, (unsigned int)10000000); 
	
	for(CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
	{
		CvBlob *blob= (*it).second;
//      printf("%d %d %d %d\n", blob->minx, blob->miny, blob->maxx, blob->maxy);
//        
//      it references the original image. but it can uses limit by area.
		Mat roi = (frame)(Rect(blob->minx, blob->miny, blob->maxx - blob->minx, blob->maxy - blob->miny));
		images.push_back(roi); 
	}
} 