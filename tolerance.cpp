#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])


IplImage* detectthis(IplImage *img, int b,int g,int r,int b1, int g1, int r1)
{
	int i,j,ht,wd,flag;
	ht=img->height;
	wd=img->width;

	IplImage* grey=cvCreateImage(cvSize(wd,ht),IPL_DEPTH_8U,1);

	for(i=0;i<ht;i++){
		for(j=0;j<wd;j++){
			flag=0;
			if(PIX(img,i,j,0)>b-b1&&PIX(img,i,j,0)<b+b1)
				if(PIX(img,i,j,1)>g-g1&&PIX(img,i,j,1)<g+g1)
					if(PIX(img,i,j,2)>r-r1&&PIX(img,i,j,2)<r+r1)
						flag=1;
			PIX(grey,i,j,0)=(flag)?255:0;
		}
	}
	return grey;
}

int main()
{
	int b=127,g=127,r=127,b1=10,g1=10,r1=10;
	CvCapture* capture=cvCreateCameraCapture(0);
	IplImage *frame,*blue;

	char win[]="video_stream";
	char win2[]="OUTPUT";

	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvNamedWindow(win2,CV_WINDOW_AUTOSIZE);


	cvCreateTrackbar("B",win2,&b,255);
	cvCreateTrackbar("G",win2,&g,255);
	cvCreateTrackbar("R",win2,&r,255);
	cvCreateTrackbar("B Tolerance",win2,&b1,127);
	cvCreateTrackbar("G Tolerance",win2,&g1,127);
	cvCreateTrackbar("R Tolerance",win2,&r1,127);


	while(1)
	{
		frame = cvQueryFrame(capture);
		blue = detectthis(frame, b,g,r,b1,g1,r1);

		cvShowImage(win,frame);
		cvShowImage(win2,blue);

		char c=cvWaitKey(33);
		if(c==27)break;
	}

	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);

	cvReleaseImage(&blue);

	cvDestroyWindow(win);
	cvDestroyWindow(win2);
	return 0;
}