#include "main.hpp"

using namespace std;
using namespace cvb;

// print mat for debuging
void printMat( cv::Mat &mat, const char *output_path ) {
	ofstream output( output_path );
	int i, j;

	for( i=0; i<mat.rows; i++ ) {
		for( j=0; j<mat.cols; j++ ) {
			output << (int)mat.at<uchar>( cv::Point(j, i) ) << " ";
		}
		output << endl;
	}
}

// get distance between two point
inline long double getDistanceTwoPoint( const cv::Point& p1, const cv::Point& p2 ) {
	cv::Point p3;
	p3 = p1-p2;
	return cv::sqrt( (long double)(p3.x)*(p3.x)+(p3.y)*(p3.y) );
}

void filterByInertia()
{

}


void filterByCircularity()
{

}

void eunchan() {
	
	cv::Mat origin, thresh, edge, dist, graph, fury, lpf, skel;
	vector< vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;
	cv::Point center;
	cv::Point point_first;
	cv::Point point_last_contour;
	cv::Point point_search_contour;
	int search_dist=1;
	int search_contour_mode;
	string file_path = "image/skel7.png";
	string output_path = "output.txt";
	ofstream file(output_path);
	unsigned int i, j;
	long double max_distance;
	uchar p;
	uchar p1;
	uchar *p2;
	bool isIncrease = true;
	vector<uchar> highest; 
	bool first=false;
	vector<uchar> vector_graph_distance;
	vector<cv::Point> vector_graph_point;
	int max_highest;
	bool can_find_contour;
	
	// for sorting
		struct _sort1 {
			bool operator() (uchar i, uchar j) { return i>j; } ;
		} sort1;

	// create window
		cv::namedWindow( "1" ); 
		cv::namedWindow( "2" );
	
	//file read, threshold and canny
		origin = cv::imread( file_path );
		cv::cvtColor( origin, thresh, CV_BGR2GRAY );
		cv::threshold( thresh, thresh, 127, 255, cv::THRESH_BINARY ); 
		//cv::Canny( thresh, edge, 127, 255, 5, true);
		edge = thresh;

	//get center
		cv::Moments mts = cv::moments( edge );

		center.x = (int)(mts.m10/mts.m00);
		center.y = (int)(mts.m01/mts.m00);

	 //calculate distance from center
		if( true ) {
			dist = cv::Mat( edge.size(), edge.type() );
			max_distance = getDistanceTwoPoint( cv::Point(0, 0), cv::Point(edge.cols, edge.rows) );
		
			for( j=0; j<edge.rows; j++ ) {
				for( i=0; i<edge.cols; i++ ) {
					p = edge.at<uchar>( cv::Point(i, j) );
					p2 = &dist.at<uchar>( cv::Point(i, j) );
					if( p==255 ) {
						if( !first ) {
							point_first.x = i;
							point_first.y = j;
							first = true;
						}
						*p2 = (uchar)( getDistanceTwoPoint(cv::Point(i, j), center)/(max_distance/256) );
					} else {
						*p2 = 0;
					}
				}
			}  
		}
	
	// star skeletonization
		// convert contours to graph
		if( true ) {
			point_last_contour = point_first;
			search_contour_mode = 0; 
			search_dist = 1;
			can_find_contour = true;
			/*
				search_contour_mode
				?	?	?	?	?
				?	1	0	7	?
				?	2	x	6	?
				?	3	4	5	?
				?	?	?	?	?
			*/ 
			do {
				// find next contour
					for( i=0; i<8 && can_find_contour; i++ ) {
						search_contour_mode+i > 7 ? j=(search_contour_mode+i-8) : j=search_contour_mode+i ;
						point_search_contour = point_last_contour;

						switch( j ) {
						case 0:
							point_search_contour.y-=search_dist; break;
						case 1:
							point_search_contour.x-=search_dist;
							point_search_contour.y-=search_dist; break;
						case 2:
							point_search_contour.x-=search_dist; break;
						case 3:
							point_search_contour.x-=search_dist;
							point_search_contour.y+=search_dist; break;
						case 4:
							point_search_contour.y+=search_dist; break;
						case 5:
							point_search_contour.x+=search_dist;
							point_search_contour.y+=search_dist; break;
						case 6:
							point_search_contour.x+=search_dist; break;
						case 7:
							point_search_contour.x+=search_dist;
							point_search_contour.y-=search_dist; break;
						}

						// if the point searching has out of image
							// x is out of image. find while y++
							if( point_search_contour.x < 0 || point_search_contour.x >= dist.cols || 
								point_search_contour.y < 0 || point_search_contour.y >= dist.rows ) {
									continue;
							}

						p = dist.at<uchar>( point_search_contour );

						if( p>0 ) {
							// found contour!
							point_last_contour = point_search_contour;
							switch( j ) {
							case 0: search_contour_mode = 5; break;
							case 1: search_contour_mode = 6; break;
							case 2: search_contour_mode = 7; break;
							case 3: search_contour_mode = 0; break;
							case 4: search_contour_mode = 1; break;
							case 5: search_contour_mode = 2; break;
							case 6: search_contour_mode = 3; break;
							case 7: search_contour_mode = 4; break;
							}
							vector_graph_distance.push_back( p );
							vector_graph_point.push_back( point_search_contour );
							break;
						}
					}

					if( i==8 ) {
						// can't find contour
						// increase search_dist
						search_dist++;
						cout << search_dist << endl;
					} else {
						search_dist = 1;
					}
					//file << point_search_contour.x << "," << point_search_contour.y << endl;
			} while ( point_search_contour!=point_first ); 

			// validate graph
				cout << vector_graph_distance.size() << endl;
			// draw graph
				if( false ) {
					graph.create( 1, vector_graph_distance.size(), 0 );
					for( i=0; i<vector_graph_distance.size(); i++ ) { 
						graph.at<uchar>( cv::Point(i, 0) ) = vector_graph_distance[i];
					}
				} else { // for debug, increase rows to 100
					graph.create( 300, vector_graph_distance.size(), 0 );
					for( i=0; i<vector_graph_distance.size(); i++ ) { 
						for( j=0; j<300; j++ )
							graph.at<uchar>( cv::Point(i, j) ) = vector_graph_distance[i];
					}
				}
				
		}

		// eunchan smooth
			if( false ) {
				for( i=0; i<100; i++) cv::blur( graph, graph, cv::Size(1, 3) );
			}

		// eunchan differential
			if( true ) {  
				isIncrease = true;
				for( i=1; i<graph.cols; i++ ) { 
					p = graph.at<uchar>( cv::Point(i-1, 0) );
					p1 = graph.at<uchar>( cv::Point(i, 0) );

					if( p1-p > 0 ) {
						isIncrease = true;
					} else if ( p1-p < 0 ) {
						if( isIncrease ) {
							highest.push_back( p );  
							cv::circle( graph, cv::Point(i-1, 20+p), 3, cv::Scalar(128) );
						}
						isIncrease = false;
					}	
				}	
				sort( highest.begin(), highest.end(), sort1 ); 
				for( i=0; i<highest.size(); i++ ) cout << "h[" << i << "] : " << (int)highest[i] << endl;
			}
		
		// eunchan get skeleton
			if( true ) {
				// check highest empty
					if( highest.size() < 5 ) 
						max_highest = highest.size();
					else
						max_highest = 5;

				isIncrease = true;
				for( i=1; i<graph.cols; i++ ) {
					p = graph.at<uchar>( cv::Point(i-1, 0) );
					p1 = graph.at<uchar>( cv::Point(i, 0) );

					if( p1-p > 0 ) {
						isIncrease = true;
					} else if ( p1-p < 0 ) {
						if( isIncrease ) {
							for( j=0; j<max_highest; j++ ) { 
								if( highest[j] == p ) {
									//found skeleton!
									cv::circle( edge, vector_graph_point[i], 3, cv::Scalar(128), 2 ); 
									cv::circle( graph, cv::Point(i-1, 20+p), 5, cv::Scalar(255) );
									highest[j] = 0;
									break;
								}
							}
						}
						isIncrease = false;
					}	
				} 
			}

	// show
		// draw center
			cv::circle( edge, center, 3, cv::Scalar(128), 2 );
		cv::imshow( "1", edge ); 
		cv::imshow( "2", graph );

	// wait
	cv::waitKey( 0 );

	file.close();

}
