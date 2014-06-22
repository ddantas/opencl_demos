#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "cl_image.h"

int main(int argc, char** argv){
	CL* cl = (CL*) malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);
	
	IplImage* img = NULL;
	img = cvLoadImage(argv[1], atoi(argv[2]));
	
	cvNamedWindow("test cv image", CV_WINDOW_AUTOSIZE);
	clInvert2D(cl, img);
	cvMoveWindow("test cv image", 0,0);
	cvShowImage("test cv image", img);
	
	cvWaitKey(0);
	
	return 0;
}
