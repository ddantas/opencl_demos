#include <stdio.h>
#include "image.h"
#include "opencl.h"
#include <stdlib.h>
#include <opencv2/core/types_c.h>

int main(int argc, char** argv){
	IplImage* img = ImRead(argv[1]);
	int tam = img->width*img->height;
	char* imageData = (char*)malloc(tam*sizeof(char));
	for(int i=0; i<tam; i++)
		imageData[i] = img->imageData[i];
	cl_ImInvert(imageData, tam);
	for(int i=0; i<tam; i++)
		img->imageData[i] = imageData[i];
	ImWrite(img, argv[2]);
	return 0;
}
