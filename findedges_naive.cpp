#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

void clearnoise(IplImage *frame, int action, int kernel){
	int i, j, black, white, i2, j2, k;
	int ht = frame->height;
	int wd = frame->width;
	for(i = 0; i< ht; i++){
		for(j = 0; j< wd; j++){
			i2 = i-2;
			j2 = j-2;
			for(k=1; k<9; k++){
				if(i2<0 || i2>=ht || j2<0 || j2>=wd)
					continue;
				if(PIX(frame,i2+(k/3),j2+(k%3),0)==0)
					black++;
				else white++;
				if(!action)
					if(white>black)
						PIX(frame,i2+(k/3),j2+(k%3),0)=255;
				else
					if(black>white)
						PIX(frame,i2+(k/3),j2+(k%3),0)=0;
			}
		}
	}
}

int pixelcheck(IplImage* img,int thresh,int x,int y)
{
	int i,j,max,min;
	min=256;
	max=-1;
	for(i=x-1;i<=x+1;i++)
	{	
		if(i<0 || i>=img->height)
			continue;
		for(j=y-1;j<=y+1;j++)
		{   
			if(j<0 || j>=img->width)
				continue;
			if(PIX(img,i,j,0)<min)
				min=PIX(img,i,j,0);
			if(PIX(img,i,j,0)>max)
				max=PIX(img,i,j,0);
		}
	}
	if((max-min)>thresh)
		return 1;
	return 0;
}

IplImage* findedge(IplImage* img,int thresh)
{
	int i,j,ht,wd;
	ht=img->height;
	wd=img->width;

	IplImage* res;
	res=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);

	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			PIX(res,i,j,0)=(uchar)(.33*PIX(img,i,j,0)+.56*PIX(img,i,j,1)+.11*PIX(img,i,j,2));

	for(i=1;i<ht-1;i++)
		for(j=1;j<wd-1;j++)
			PIX(res,i,j,0)=(pixelcheck(img,thresh,i,j))?0:255;
	return res;
}

int main()
{
	int thresh=25;
	CvCapture* capture;
	capture = cvCreateCameraCapture(-1);
	IplImage *frame,*res;
	char win[]="video_stream";
	char win2[]="outline_feed";
	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvNamedWindow(win2,CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Threshold",win2,&thresh,128);

	while(1)
	{
		frame=cvQueryFrame(capture);
		res = findedge(frame,thresh);
		clearnoise(res,0, 3);
		clearnoise(res,1, 3);
		cvShowImage(win,frame);
		cvShowImage(win2,res);
		
		if(cvWaitKey(50)==27)
			break;
	}
	cvReleaseImage(&frame);
	cvReleaseImage(&res);
	cvReleaseCapture(&capture);
	cvDestroyWindow(win);
	cvDestroyWindow(win2);
	return 0;
}