#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "cl_image.h"
/*
IplImage* simuleIpl(VglImage* vgl){
	IplImage* img = (IplImage*)malloc(sizeof(IplImage));
	img->nSize		=	vgl->ipl->nSize;
	img->ID			=	0;
	img->nChannels	=	vgl->nChannels;
	img->alphaChannel = vgl->ipl->alphaChannel;
	img->depth		=	vgl->ipl->depth;
	img->dataOrder	=	vgl->ipl->dataOrder;
	img->origin		=	vgl->ipl->origin;
	img->align		=	vgl->ipl->align;
	img->width		=	vgl->shape[0];
	img->height		=	vgl->shape[1]*vgl->shape[2];
	img->imageSize	=	vgl->shape[0]*vgl->shape[1]*vgl->shape[2]*vgl->nChannels;
	img->widthStep	=	vgl->shape[0]*vgl->nChannels;
	char* b = (char*)malloc(img->imageSize);
	for(int i=0; i<(vgl->shape[0]*vgl->nChannels*vgl->shape[1]*vgl->shape[2]); i++)
		b[i] = vgl->ndarray[i];
	img->imageData = b;
	return img; 
}
*/

void RGBA2RGB(IplImage* img){
	char* b = (char*)malloc(img->height*img->width*3);
	int i=0, j=0;
	for(int k=0; k<(img->width*img->height); k++){
		b[i] = img->imageData[j]; i++; j++;
		b[i] = img->imageData[j]; i++; j++;
		b[i] = img->imageData[j]; i++; j++;
		j++;
	}
}

int main(int argc, char** argv){
	CL* cl = (CL*) malloc(sizeof(CL));
	StartCL(cl);
	StartQueue(cl);
	
	VglImage* vgl = NULL;
	vgl = getVglImage(argv[1]);
	printf("consegui a vgl\n");
	//clInvert3D(cl, vgl);//lembrar de liberar a memoria
	vgl->ipl->imageData=vgl->ndarray;
	vgl->ipl->height*=vgl->shape[2];
	rgb2rgba(vgl->ipl, NULL);
	RGBA2RGB(vgl->ipl);
	int p[3];
	p[0] = CV_IMWRITE_JPEG_QUALITY;
    p[1] = 100;
    p[2] = 0;
	cvSaveImage("./output/teste.jpg", vgl->ipl, p);
	printf("Imagem salva em ./output/teste.jpg\n");
		
	return 0;
}
