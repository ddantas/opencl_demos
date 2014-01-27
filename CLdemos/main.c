#include "cl_image.h"

int main(int argc, char** argv){
	CL* cl=(CL*)malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);
	
	IplImage* img = (IplImage*)malloc(sizeof(IplImage));
	img = ImRead(argv[1]);
	cl_ImInvert(cl,img);
	ImWrite(img, argv[2]);
	unsigned char thre = 100;
	cl_ImThreshold(cl, img, thre);
	ImWrite(img, argv[3]);
	return 0;
}
