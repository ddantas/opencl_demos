#include "image.h"

int main(int argc, char** argv){
	IplImage *img = img_read(argv[1]);
	int n;
	unsigned char c;
	while (!((n>=0)&&(n<=255))){
		printf("Type a number: ");
		scanf("%d", &n);
		printf("\n");
	}
	c = (unsigned char) n;
	img_threshold(img, c);
	img_write(img, argv[2]);
	return 0;
}