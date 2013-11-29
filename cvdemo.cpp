#include <stdio.h>
#include "opencv2/opencv.hpp"
 
int main()
{
CvCapture* capture = cvCaptureFromCAM(-1);
if(!capture){
	printf("camera not connected");
	cvWaitKey(0);
}
IplImage *real;
cvNamedWindow("C", CV_WINDOW_AUTOSIZE);
do{
real=cvQueryFrame(capture);	
cvShowImage("C", real);
if(cvWaitKey(33)==27)break;
}while(1);
	  
cvReleaseImage(&real);
cvDestroyWindow("CV");
}