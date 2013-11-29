// #include <stdio.h>
#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

IplImage* greyit(IplImage *img, int thresh)
{
int i,j,ht,wd;
ht=img->height;
wd=img->width;

IplImage* grey=cvCreateImage(cvSize(wd,ht),IPL_DEPTH_8U,1);
for(i=0;i<ht;i++)
	for(j=0;j<wd;j++)
	{
		PIX(grey,i,j,0)=(uchar)(.33*PIX(img,i,j,0)+.56*PIX(img,i,j,1)+.11*PIX(img,i,j,2));
		if(PIX(grey,i,j,0)>thresh)
			PIX(grey,i,j,0)=255;
		else PIX(grey,i,j,0)=0;
	}
return grey;
}

int main()
{
	int thresh = 127;
	CvCapture* capture=cvCreateCameraCapture(0);
	IplImage *frame,*grey;
	char win[]="video_stream";
	char win2[]="trackbar";
	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvNamedWindow(win2,CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("trackbarwin",win2,&thresh,255);
	while(1)
	{
		frame = cvQueryFrame(capture);
		grey = greyit(frame, thresh);
		cvShowImage(win,frame);
		cvShowImage(win2,grey);
		char c=cvWaitKey(33);
		if(c==27)break;
	}
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvReleaseImage(&grey);
	cvDestroyWindow(win);
	cvDestroyWindow(win2);
	return 0;
}