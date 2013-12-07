#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

int main()
{
	int low=0,high=128;
	CvCapture* capture;
	capture = cvCreateCameraCapture(-1);
	IplImage *frame,*grey,*res;
	char win[]="video_stream";
	char win2[]="outline_feed";
	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvNamedWindow(win2,CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Low",win2,&low,128);
	cvCreateTrackbar("High",win2,&high,128);

	frame=cvQueryFrame(capture);
	grey=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
	res=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);

	while(1)
	{
		frame=cvQueryFrame(capture);
		cvCvtColor(frame,grey,CV_BGR2GRAY);
		cvCanny(grey,res,low,high,3);
		cvShowImage(win,frame);
		cvShowImage(win2,res);
		char c=cvWaitKey(33);
		if(c==27)
			break;
	}
	cvReleaseImage(&frame);
	cvReleaseImage(&res);
	cvReleaseCapture(&capture);
	cvDestroyWindow(win);
	cvDestroyWindow(win2);
	return 0;
}