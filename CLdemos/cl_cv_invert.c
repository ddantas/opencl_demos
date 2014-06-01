#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "cl_image.h"

int main(int argc, char** argv){
	CL* cl = (CL*) malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);
	
	cvNamedWindow("test cv image", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	
	IplImage* frame;
	IplImage* gray_frame;
	
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		gray_frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, gray_frame, CV_RGB2GRAY);
		cl_ImInvert2(cl, gray_frame);
		cvShowImage("test cv image", gray_frame);
		char c = cvWaitKey(33);
		if(c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("test cv image");
	return 0;
}
