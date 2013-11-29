#include <stdio.h>
#include "opencv2/opencv.hpp"

int main()
{
	IplImage* img = cvLoadImage("./arrow_simple.png", 0);
	cvNamedWindow("win",0);
	cvShowImage("win", img);
	cvWaitKey(0);
}