#include "image.h"


int main(int argc, char** argv){
	IplImage *img = ImRead(argv[1]);
	ImInvert(img);
	ImWrite(img, argv[2]);
	return 0;
}
