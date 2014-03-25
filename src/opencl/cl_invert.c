#include "cl_image.h"

int main(int argc, char **argv){
	
	IplImage *img = (IplImage *) malloc (sizeof(IplImage));
	CL *cl = (CL *) malloc (sizeof(CL));
	startCL(cl);
	startQueue(cl);
	
	img = img_read(argv[1]);
	printf("Reading image...\n");
	cl_invert(cl, img);
	img_write(img, argv[2]);
	
	return 0;
}