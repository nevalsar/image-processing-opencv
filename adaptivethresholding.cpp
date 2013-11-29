#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

int findmedian(IplImage* img)
{
	int intensities[256];
	int i,j,temp=0;
	for(i=0;i<256;i++)
		intensities[i]=0;

	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			intensities[PIX(img,i,j,0)]++;

	for(i=0;i<256;i++){
		temp += intensities[i];
		if(temp > (img->width*img->height)/2)
			break;
	}
	return i;
}

IplImage* createbinary(IplImage *img)
{
	int i,j,ht,wd,thresh;
	ht=img->height;
	wd=img->width;

	IplImage* output = cvCreateImage(cvSize(wd,ht),IPL_DEPTH_8U,1);
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			PIX(output,i,j,0)=(uchar)(.33*PIX(img,i,j,0)+.56*PIX(img,i,j,1)+.11*PIX(img,i,j,2));

	// now output is a greyscale image

	thresh = findmedian(output);
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			PIX(output,i,j,0)=(PIX(output,i,j,0)>thresh)?255:0;
	//  output now has been converted to a binary image
	return output;
}



int main()
{
	CvCapture* capture=cvCreateCameraCapture(0);
	IplImage *frame,*output;
	char win[]="video_stream";
	char win2[]="outputscale";

	cvNamedWindow(win,CV_WINDOW_AUTOSIZE);
	cvNamedWindow(win2,CV_WINDOW_AUTOSIZE);

	while(1)
	{
		frame = cvQueryFrame(capture);
		output = createbinary(frame);
		cvShowImage(win,frame);
		cvShowImage(win2,output);
		char c=cvWaitKey(33);
		if(c==27)
			break;
	}

	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvReleaseImage(&output);
	cvDestroyWindow(win);
	cvDestroyWindow(win2);
	return 0;
}