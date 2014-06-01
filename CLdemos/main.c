#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "cl_image.h"

int main(int argc, char** argv){
	CL* cl = (CL*) malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);

	IplImage* img2 = (IplImage*)malloc(sizeof(IplImage));
	img2 = ImRead(argv[1]);
	
	unsigned char thre = 100;
	cl_ImThreshold(cl, img2, thre);
	ImWrite(img2, argv[3]);
	
	free(img2);
	
	IplImage* img = (IplImage*)malloc(sizeof(IplImage));
	img = ImRead(argv[1]);
	
	cl_ImInvert(cl,img);
	ImWrite(img, argv[2]);

	free(img);
	return 0;
}
