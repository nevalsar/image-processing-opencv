#include <stdio.h>
#include "opencv2/opencv.hpp"
 
int main()
{
	IplImage* img;
	char win[] = "Striped Image";
	img = cvCreateImage(cvSize(200,300),IPL_DEPTH_8U,3);
	uchar *imgdat = (uchar*)img->imageData;
	int location;
 
	for(int i=0; i < img->height; i++) {
		for(int j=0; j < img->width; j++) {
			location = i*img->widthStep+j*img->nChannels;
			imgdat[location] = imgdat[location+1] = imgdat[location+2] = 0;
			if(i < img->height/3)
				imgdat[location] = 255;		//blue
			else if(i >= (2*img->height)/3)
				imgdat[location+2] = 255;	//red
			else
				imgdat[location+1] = 255;	//green
		}
	}
 
	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvShowImage(win,img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow(win);
	return 0;
}