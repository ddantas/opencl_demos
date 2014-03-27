#include "cl_image.h"

int main(int argc, char** argv){
	
	IplImage *img = (IplImage*) malloc (sizeof(IplImage));
	CL *cl = (CL*) malloc (sizeof(CL));
	img = img_read(argv[1]);
	startCL(cl);
	startQueue(cl);
	cl_invert_2d(cl, img);
	img_write(img, argv[2]);
	
	return 0;
}