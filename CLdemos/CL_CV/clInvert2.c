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
	img = cvLoadImage(argv[1], 0);
	
	cvNamedWindow("test cv image", CV_WINDOW_AUTOSIZE);
	cl_ImInvert2(cl, img);
	cvShowImage("test cv image", img);
	
	cvWaitKey(0);
	
	return 0;
}
