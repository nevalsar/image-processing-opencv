#include "opencv2/opencv.hpp"
#include <stdio.h>

int mx,my,chk=0;

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

void mouse_callback(int event,int x, int y, int flags, void *param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		chk=1;
		mx=x;
		my=y;
	}
}

int main()
{
CvCapture* capture=cvCreateCameraCapture(0);
IplImage *frame,*grey;
char win[] = "video_stream";
cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
cvSetMouseCallback(win,mouse_callback,0);
while(1)
{
frame = cvQueryFrame(capture);
cvShowImage(win,frame);
if (chk==1)
	{
		chk=0;
		printf("X: %d, Y: %d  --- ",mx,my);
		printf("Colour B: %d\tG: %d\tR: %d\n",PIX(frame,my,mx,0),PIX(frame,my,mx,1),PIX(frame,my,mx,2));
	}
char c=cvWaitKey(33);
if(c==27)break;
}
cvReleaseCapture(&capture);
cvReleaseImage(&frame);
cvDestroyWindow(win);
return 0;
}