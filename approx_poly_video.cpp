#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

#define PIX(img, i, j, k) ( img.at<Vec3b>(i,j)[k] )

int returnEdgeno(Mat frame, Mat res){
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(frame, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	vector< vector<Point> > final_contours(contours.size());
	vector<Rect> boxes(contours.size());
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
	for(int i=0 ; i< contours.size(); i++){
		approxPolyDP(Mat(contours[i]), final_contours[i], 3, true);
		boxes[i] = boundingRect(Mat(final_contours[i]));
		minEnclosingCircle((Mat)final_contours[i], center[i], radius[i]);
	}

	RNG rng(12345);
	for( int i = 0; i< contours.size(); i++ ){
    	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    	drawContours( res, final_contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    	// rectangle( res, boxes[i].tl(), boxes[i].br(), color, 2, 8, 0 );
    	// circle( res, center[i], (int)radius[i], color, 2, 8, 0 );
    }
	return final_contours.size();
}

int main(int argc, char *argv[]){
	VideoCapture capture(0);
	Mat input, gray;
	int thresh = 125;
	capture >> input;
	Mat res = Mat::zeros( input.size(), CV_8UC3 );
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	namedWindow("output", CV_WINDOW_AUTOSIZE);
	createTrackbar("thresh", "input", &thresh, 255);
	while(1){
		capture >> input;
		if(!input.data){
			cout<<"No image loaded!\n";
			getchar();
			exit(0);
		}
		res = Scalar(0,0,0);
		cvtColor(input, gray, CV_BGR2GRAY);
		threshold(gray, gray, thresh, 255, 0);
		imshow("input",gray);
		int x = returnEdgeno(gray, res);
		imshow("output", res);
		if(waitKey(33) == 32)
			break;
	}
	return 0;
}
