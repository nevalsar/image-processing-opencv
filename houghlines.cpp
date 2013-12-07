#include "opencv2/opencv.hpp"

using namespace std;

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

void converttobin(IplImage *img)
{
	int i, j,ht, wd;
	ht=img->height;
	wd=img->width;
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			PIX(img, i,j,0)=(PIX(img,i,j,0)==255)?0:255;
			
}

void plotpoint(IplImage* res, int y, int x)
{
	int t;
	for(t=0; t<500; t++)
	{
		float theta = (6.28/500)*t;
		int rad =(int)((x*cos(theta) + y*sin(theta)));
		if(rad >= res->height || rad < 0 || t >= res->width || t < 0) 
		{
			//printf("error\n");
			continue;
		}
		PIX(res, rad, t, 0)++;
	}
}

void detectcircles(IplImage *res,IplImage* res2, IplImage* bin)
{
	int i,j,ht,wd,max;
	wd=bin->width;
	ht=bin->height;

	for(i=0;i<ht;i++)
	{
		for(j=0;j<wd;j++)
		{
			if(PIX(bin,i,j,0)==255)
				plotpoint(res,i,j);
		}
	}
	max=PIX(res,0,0,0);
	for(i=0;i<700;i++)
		for(j=0;j<500;j++)
		{
			if(max<PIX(res,i,j,0))
				max=PIX(res,i,j,0);
		}
	// printf("max = %d\n", max); 
		cout<<"max = "<<max<<endl;

//scaling the ditribution;
for(i=0;i<700;i++)
	for(j=0;j<500;j++)
	{
		PIX(res2,i,j,0)=(PIX(res,i,j,0)/(float)max)*255;
	}
}

int main()
{
	//CvCapture *capture;
	//capture=cvCreateCameraCapture(-1);
	IplImage *frame,*grey,*res,*res2;
	int i,j;
	frame=cvLoadImage("lines.png",1);
	grey=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
	res=cvCreateImage(cvSize(500,700),32,1);
	res2=cvCreateImage(cvSize(500,700),IPL_DEPTH_8U,1);
	for(i=0;i<700;i++)
		for(j=0;j<500;j++)
			{
				PIX(res,i,j,0)=0;
				PIX(res2,i,j,0)=0;
			}

	char win[]="binarymatrix";
	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	//frame=cvQueryFrame(capture);
	cvCvtColor(frame, grey,CV_BGR2GRAY);
	converttobin(grey); //this function also inverts image along with conversion to binary
	cvNamedWindow("grey",CV_WINDOW_AUTOSIZE);
	cvShowImage("grey", grey);
	detectcircles(res,res2,grey); // grey has already been converted to binary and inverted
	cvShowImage(win, res2);
	cvWaitKey(0);

	cvReleaseImage(&frame);
	cvReleaseImage(&grey);
	cvReleaseImage(&res);
	cvReleaseImage(&res2);
	cvDestroyWindow(win);
	return 0;
}