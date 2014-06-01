#include <cv.h>
#include <highgui.h>
#define __CV__ true
#include "cl_image.h"

int main(int argc, char** argv){
	/*
	CL* cl = (CL*) malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);
	
	cvNamedWindow("CL CV INVERT TEST", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	while(1){
		frame = cvQueryFrama(capture);
		if(!frame)
			break;
		cl_ImInvert2(cl, frame);
		cvShowImage("CL CV INVERT TEST", frame);
		char c = cvWaitKey(33);
		if(c == 27)
			break;
	}
	
	cvReleaseCapture(&capture);
	cvDestroyWindow("CL CV INVERT TEST");
	*/
	return 0;
}

