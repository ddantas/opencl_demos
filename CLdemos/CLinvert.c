#include "invert.h"
#include "image.h"

int main(int argc, char** argv){
	IplImage* img;
	img = ImRead(argv[1]);
	unsigned char depth = 255;
	clinvert(img->imageData, depth, (img->width*img->height));
	ImWrite(img, argv[2]);
	return 0;
}


//gcc -I/opt/AMDAPP/include -I. main.c image.o invert.o -L/opt/AMDAPP/lib/x86 -lOpenCL -o main -lm -std=gnu99
