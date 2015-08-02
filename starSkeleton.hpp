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

void swap( vector<uchar> &vector_uchar, vector<cv::Point> &vector_point, int a, int b ) {
	uchar tmp_uchar;
	cv::Point tmp_point;

	tmp_uchar = vector_uchar[a];
	vector_uchar[a] = vector_uchar[b];
	vector_uchar[b] = tmp_uchar;

	tmp_point = vector_point[a];
	vector_point[a] = vector_point[b];
	vector_point[b] = tmp_point;
}

/**
* Find and return the index of pivot element.
* @param a - The array.
* @param first - The start of the sequence.
* @param last - The end of the sequence.
* @return - the pivot element
*/
int pivot( vector<uchar> &vector_uchar, vector<cv::Point> &vector_point, int first, int last) 
{
	int  p = first;
	uchar pivotElement = vector_uchar[first];

	for(int i = first+1 ; i <= last ; i++)
	{
		/* If you want to sort the list in the other order, change "<=" to ">" */
		if(vector_uchar[i] > pivotElement)
		{
			p++;
			swap( vector_uchar, vector_point, i, p );
		}
	}

	swap( vector_uchar, vector_point, p, first );

	return p;
}

/**
* Quicksort.
* @param a - The array to be sorted.
* @param first - The start of the sequence to be sorted.
* @param last - The end of the sequence to be sorted.
*/
void quickSort_derivative( vector<uchar> &vector_uchar, vector<cv::Point> &vector_point, int first, int last ) 
{
	int pivotElement;

	if( first < last ) {
		pivotElement = pivot( vector_uchar, vector_point, first, last );
		quickSort_derivative(vector_uchar, vector_point, first, pivotElement-1);
		quickSort_derivative(vector_uchar, vector_point, pivotElement+1, last); 
	}
}



/*
starSkeleton
input_mat : should be gray-scale and treshold
output_mat : pointing at features as many as 3rd parameter (temporary)
point_cnt : pointing count
return : 1 - success
0 - has occured unkown error (but never give an info about what error occured)
*/
int starSkeleton(cv::Mat &input_mat, cv::Mat &output_mat, cv::Point &head, cv::Point &left, cv::Point &right, cv::Point &center, unsigned int huristic_distance = 20) {

	cv::Mat origin, graph, skel;
	cv::Point point_center;
	unsigned int i, j; 
	vector<int> vector_graph_distance;
	vector<cv::Point> vector_graph_point;
	vector<int> vector_derivative;
	vector<cv::Point> vector_derivative_point;
	cv::Scalar color_head;
	cv::Scalar color_left_hand;
	cv::Scalar color_right_hand;
	cv::Scalar color_center; 
	unsigned int head_idx; 
	unsigned int left_hand_idx; 
	unsigned int right_hand_idx;
	int circle_size;
	int circle_thickness;

	// init
	origin = input_mat;
	struct _sort1 {
		bool operator() (uchar i, uchar j) { return i>j; } ;
	} sort1;
	cv::cvtColor( origin, skel, CV_GRAY2BGR );
	color_head = cv::Scalar( 0, 255, 255, 0 );
	color_left_hand = cv::Scalar( 0, 0, 255, 0 );
	color_right_hand = cv::Scalar( 255, 0, 0, 0 );
	color_center = cv::Scalar( 128, 128, 128, 0 );
	circle_size = 5;
	circle_thickness = 3;

	//get center
	cv::Moments mts = cv::moments( origin );

	point_center.x = (int)(mts.m10/mts.m00);
	point_center.y = (int)(mts.m01/mts.m00);

	// contours to distance graph
	if( true ) {
		cv::Point point_first;
		cv::Point point_last_contour;
		cv::Point point_search_contour;
		int search_contour_mode = 0; 
		int search_dist = 1;
		bool can_find_contour = true;
		for( j=0; j<origin.rows; j++ ) {
			for( i=0; i<origin.cols; i++ ) {
				if( origin.at<uchar>( cv::Point(i, j) ) > 0 ) {
					point_last_contour = point_first = cv::Point(i, j);
					goto I_FOUND_FIRST_CONTOUR;
				}
			}
		}
I_FOUND_FIRST_CONTOUR:						

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
				if( point_search_contour.x < 0 || point_search_contour.x >= origin.cols || 
				   point_search_contour.y < 0 || point_search_contour.y >= origin.rows ) {
					continue;
				}

				//p = dist.at<uchar>( point_search_contour );
				uchar u = origin.at<uchar>( point_search_contour );

				if( u>0 ) {
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
					vector_graph_distance.push_back( getDistanceTwoPoint( point_center, point_search_contour ) );
					vector_graph_point.push_back( point_search_contour );
					break;
				}
			}
			// can't find contour
			// increase search_dist
			if( i==8 )		
				search_dist++;
			else
				search_dist = 1;
		} while ( point_search_contour!=point_first );  
	}

	// eunchan differential
	if( true ) {  
		int isIncrease = true;
		for( i=1; i<vector_graph_distance.size(); i++ ) { 
			int p1 = vector_graph_distance[i-1];
			int p2 = vector_graph_distance[i];

			if( p2-p1 > 0 ) {
				isIncrease = true;
			} else if ( p2-p1 < 0 ) {
				if( isIncrease ) {  
					for( j=0; j<vector_derivative.size(); j++ ) {
						if( getDistanceTwoPoint( vector_graph_point[i], vector_derivative_point[j] ) <= huristic_distance ) 
							if( vector_derivative[j] > p1 )
								break;
							else { // new point has farther.
								vector_derivative.erase( vector_derivative.begin()+j );
								vector_derivative_point.erase( vector_derivative_point.begin() + j );
								j--;
							}
					}
					if( j==vector_derivative.size() ) {
						vector_derivative.push_back( p1 );
						vector_derivative_point.push_back( vector_graph_point[i] );							 
					}
				}
				isIncrease = false;
			}	
		}	
	}

	// if less then point_cnt, return 0;
	if( vector_derivative.size() < 3 )
		return 0;

	// derivative huristic algorithm 
	if (true) {
		cv::Point new_point;
		cv::Point old_point;
		cv::Point head_point;
		cv::Point leftest_point;
		cv::Point rightest_point;
		int leftest_idx;
		int rightest_idx;

		// find where is the head 
		head_idx = 0;
		leftest_idx = 0;
		rightest_idx = 0;

		for (i = 1; i<vector_derivative.size(); i++) {
			new_point = vector_derivative_point[i];
			old_point = vector_derivative_point[head_idx];
			leftest_point = vector_derivative_point[leftest_idx];
			rightest_point = vector_derivative_point[rightest_idx];
			int new_x = abs(new_point.x - point_center.x);
			int old_x = abs(old_point.x - point_center.x);

			// leftest
			if (new_point.x < leftest_point.x)
				leftest_idx = i;

			// rightest
			if (new_point.x > rightest_point.x)
				rightest_idx = i;

			// find head
			if (new_x < huristic_distance) { 
				if (old_x > huristic_distance)
					head_idx = i;
				else if (old_point.y > new_point.y) // if new point is higher than old point.
					head_idx = i;
			}
		}
		head_point = vector_derivative_point[head_idx];

		// find left hand
		left_hand_idx = 0;
		for (i = 1; i<vector_derivative.size(); i++) {
			new_point = vector_derivative_point[i];
			old_point = vector_derivative_point[left_hand_idx]; 
			int old_dist = getDistanceTwoPoint(point_center, old_point);
			int new_dist = getDistanceTwoPoint(point_center, new_point);

			if (head_point.x > new_point.x)
				if (abs(old_point.x - leftest_point.x) > huristic_distance)
					left_hand_idx = i;
				else if (abs(new_point.x - leftest_point.x) < huristic_distance) 
					if (old_dist < new_dist)
						left_hand_idx = i;
		}

		// find right hand 
		right_hand_idx = 0;
		for (i = 1; i < vector_derivative.size(); i++) {
			new_point = vector_derivative_point[i];
			old_point = vector_derivative_point[right_hand_idx];
			int old_dist = getDistanceTwoPoint(point_center, old_point);
			int new_dist = getDistanceTwoPoint(point_center, new_point);

			if (head_point.x < new_point.x)
				if (abs(old_point.x - rightest_point.x) > huristic_distance)
					right_hand_idx = i;
				else if (abs(new_point.x - rightest_point.x) < huristic_distance)
					if (old_dist < new_dist)
						right_hand_idx = i;
		}
	}

	// ins't there any point? return 0;
	if (vector_derivative_point.size() == 0)
		return 0;

	// return
	cv::circle( skel, point_center, circle_size, color_center, circle_thickness );
	cv::circle( skel, vector_derivative_point[head_idx], circle_size, color_head, circle_thickness);
	cv::circle( skel, vector_derivative_point[left_hand_idx], circle_size, color_left_hand, circle_thickness);
	cv::circle( skel, vector_derivative_point[right_hand_idx], circle_size, color_right_hand, circle_thickness);

	head = vector_derivative_point[head_idx];
	left = vector_derivative_point[left_hand_idx];
	right = vector_derivative_point[right_hand_idx];
	center = point_center;

	// draw all point
	if (false) {
		for (i = 0; i < vector_derivative.size(); i++) {
			cv::circle(skel, vector_derivative_point[i], circle_size, cv::Scalar(128), circle_thickness);
		}
	}

	output_mat = skel; 
                
	return 1;
}