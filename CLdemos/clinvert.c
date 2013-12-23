#include <stdio.h>
#include "image.h"
#include "opencl.h"
#include <stdlib.h>
#include <opencv2/core/types_c.h>

int* transformInt(char* a, int tam){
	int* b = malloc(sizeof(int)*tam);
	for(int i=0; i<tam; i++){
		b[i] = (int)a[i];
	}
	return b;
}
void transformChar(int* a, char* b, int tam){
	for(int i=0; i<tam;i++){
		b[i] = (char)a[i];
	}
}
	
int main(int argc, char** argv){
	IplImage* img = ImRead(argv[1]);
	int tam = img->width*img->height;
	char* imageData = (char*)malloc(tam*sizeof(char));
	for(int i=0; i<tam; i++)
		imageData[i] = img->imageData[i];
	teste(imageData, tam);
	for(int i=0; i<tam; i++)
		img->imageData[i] = imageData[i];
	ImWrite(img, argv[2]);
	return 0;
}
