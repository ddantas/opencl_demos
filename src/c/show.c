#include "image.h"

int main(int argc, char** argv){
	IplImage *img = img_read(argv[1]);
	img_write(img, argv[2]);
	print_console(img);
	return 0;
}