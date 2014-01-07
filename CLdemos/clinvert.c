#include <stdio.h>
#include "image.h"
#include "opencl.h"
#include <stdlib.h>
#include <opencv2/core/types_c.h>

int main(int argc, char** argv){
	IplImage* img = ImRead(argv[1]);
	int tam = img->width*img->height;
	cl_ImInvert(img->imageData, tam);
	ImWrite(img, argv[2]);
	return 0;
}
