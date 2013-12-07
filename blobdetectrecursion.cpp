#include <stdio.h>
#include "opencv2/opencv.hpp"

#define PIX(img,i,j,k) (((uchar*)img->imageData)[i*img->widthStep+j*img->nChannels+k])

void dfsrecursion(IplImage* img, int x, int y, int **status, int blobcount)
{
	int i,j;

	//traverse all connected pixels and start dfs if encounter a black unvisited pixel
	// blob count will remain unchanged since all are part of same blob
	for(i=x-1; i<=x+1; i++)
		for(j=y-1; j<=y+1; j++)
			if(PIX(img,i,j,0)==0 && status[i][j]==-1)
			{
				status[i][j] = blobcount;
				dfsrecursion(img,i,j, status, blobcount);
			}

}

void colorblobs(IplImage* img,int blobcode, int **status)
{
	int randomcolor,i,j,ht,wd;
	ht = img->height;
	wd = img->width;

	// generate a random color from 0-255 to color this blob
	randomcolor = rand() % 256;

	// if status of pixel matches the blob's code, colour it
	for(i=0; i<ht; i++)
		for(j=0; j<wd; j++)
			if(status[i][j] == blobcode)
				PIX(img, i, j, 0) = randomcolor;
	
}

void blobdetect(IplImage *img, IplImage *out)
{
	int i,j,ht,wd;
	ht=img->height;
	wd=img->width;

	int **status, blobcount = 0;

	// create 2D array for storing visited status of pixels
	status = (int**)malloc(ht*sizeof(int*));
	for(i=0; i<ht; i++)
		status[i] = (int*)malloc(wd*sizeof(int));

	//initialise status array as unvisited (-1)
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			status[i][j]=-1;

	// start DFS when encounter a black unvisited pixel
	//increase blob count and mark status of all pixel in this blob with same value
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			if(PIX(img,i,j,0)==0 && status[i][j]==-1)
				{
					status[i][j]=++blobcount;
					dfsrecursion(img,i,j, status, blobcount);
				}
	
	//color ouput image black
	for(i=0; i<ht; i++)
		for(j=0; j<wd; j++)
			PIX(out, i, j, 0) = 255;

	// color blobs in the output image with different colors
	for(i=1;i<=blobcount;i++)
		colorblobs(out,i, status);

	//deallocate allocated memory
	for(i=0; i<ht; i++)
		free(status[i]);
	free(status);
}

void converttobinary(IplImage* img)
{
	int i,j,ht,wd;
	ht=img->height;
	wd=img->width;
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
			PIX(img,i,j,0)=(PIX(img,i,j,0)==255)?255:0;
}

int main()
{
	IplImage *res, *frame, *out;

	//load image with white background (blobs can be of any color)
	frame=cvLoadImage("./input_image.png",1);  
	if(!frame){
		printf("No image loaded!\n");
		exit(0);
	}
	res = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
	out = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);

	//convert the image to greyscale and then to binary
	cvCvtColor(frame, res, CV_BGR2GRAY);  
	converttobinary(res); 

	// start blob detection on binary image with black background and white objects
	blobdetect(res, out);

	// display output
	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);
	cvShowImage("output", out);
	cvWaitKey(0);

	// free image space allocated and destroy created windows
	cvDestroyWindow("output");
	cvReleaseImage(&frame);
	cvReleaseImage(&res);
	cvReleaseImage(&out);
	return 0;
}