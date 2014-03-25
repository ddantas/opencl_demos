#include "image.h"

int main(int argc, char** argv){
	IplImage *img = img_read(argv[1]);
	img_invert(img);
	img_write(img, argv[2]);
	return 0;
}