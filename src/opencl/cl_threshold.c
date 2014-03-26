#include "cl_image.h"

int main(int argc, char **argv){
	IplImage *img = (IplImage *) malloc (sizeof(IplImage));
	CL *cl = (CL *) malloc (sizeof(CL));
	unsigned char n = 100;
	
	startCL(cl);
	startQueue(cl);
	
	img = img_read(argv[1]);
	cl_threshold(cl, img, n);
	img_write(img, argv[2]);
	
	free(img);
	free(cl);
	
	return 0;
}