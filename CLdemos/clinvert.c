#include <stdio.h>
#include "image.h"
#include "opencl.h"
#include <stdlib.h>
#include <opencv2/core/types_c.h>

int* transformInt(char* a, int tam){
	int* b = malloc(sizeof(int)*tam);
	for(int i=0; i<tam; i++){
		printf(" b[%d] = %d\n", i, a[i]);
		b[i] = (int)a[i];
	}
	return b;
}
void transformChar(int* a, char* b, int tam){
	for(int i=0; i<tam;i++){
		printf(" b[%d] = %d\n", i, a[i]);
		b[i] = (char)a[i];
	}
}
	
int main(int argc, char** argv){
	IplImage* img = ImRead(argv[1]);
	int* aux = transformInt(img->imageData, (img->height*img->width));
	aux = teste(aux, (img->height*img->width));
	transformChar(aux, img->imageData, (img->height*img->width));
	ImWrite(img, argv[2]);
	return 0;
}
