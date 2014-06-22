#include "video.h"
void startVgl(VglImage* img){
	img->ipl=(IplImage*)malloc(sizeof(IplImage));
	img->ndarray = NULL;
}
void copyIpl(IplImage* img, IplImage* img2){
	img->nSize		=	img2->nSize;
//	printf("nSize %d nSize2 %d\n", img->nSize, img2->nSize);
	img->ID			=	img2->ID;
//	printf("nSize %d nSize2 %d\n", img->ID, img2->ID);
	img->nChannels	=	img2->nChannels;
//	printf("nChannels %d nChannels2 %d\n", img->nChannels, img2->nChannels);
	img->alphaChannel = img2->alphaChannel;
//	printf("alpha %d alpha2 %d\n", img->alphaChannel, img2->alphaChannel);
	img->depth		=	img2->depth;
//	printf("depth %d depth2 %d\n", img->depth, img2->depth);
	img->dataOrder	=	img2->dataOrder;
//	printf("dataOrder %d dataOrder2 %d\n", img->dataOrder, img2->dataOrder);
	img->origin		=	img2->origin;
//	printf("origin %d origin2 %d\n", img->origin, img2->origin);
	img->align		=	img2->align;
//	printf("align %d align2 %d\n", img->align, img2->align);
	img->width		=	img2->width;
//	printf("width %d width2 %d\n", img->width, img2->width);
	img->height		=	img2->height;
//	printf("height %d height2 %d\n", img->height, img2->height);
	img->imageSize	=	img2->imageSize;
//	printf("imgSize %d imgSize2 %d\n", img->imageSize, img2->imageSize);
	img->widthStep	=	img->width*img->nChannels;
//	printf("widthStep %d\n", img->widthStep);
}
void addNDarray(char* imageData, VglImage* img, int frame){
	int tam = img->shape[0]*img->shape[1]*img->nChannels;
	if(frame==1){
		img->ndarray=(char*)malloc(sizeof(char)*tam);
		for(int i=0; i<tam; i++)
			img->ndarray[i] = imageData[i];
	}
	if(frame>1){
		img->ndarray=(char*)realloc(img->ndarray, (sizeof(char)*tam*(frame+1)));
		for(int i=0, j=tam*frame; i<tam; i++, j++)
			img->ndarray[j] = imageData[i];
	}
}
void VglInfo(VglImage* img){
	for(int i=0; i<MAX_DIM; i++)
		printf("shape[%d] = %d\n", i, img->shape[i]);
	printf("Ndim %d\n", img->ndim);
	printf("Nchannels %d\n", img->nChannels);
	//for(int i=0; i<img->shape[0]*img->shape[1]*img->shape[2]*img->nChannels; i++)
		//printf("%d ", (int)img->ndarray[i]);
	printf("\n");
}
VglImage* getVglImage(const char* filename){
	CvCapture* 	capture = cvCaptureFromAVI(filename);
	if( !capture ){
		printf("Error when reading steam_avi\n");
		exit;
	}
	IplImage* 	frame=0; int nframes=0;
	VglImage*	vgl=(VglImage*)malloc(sizeof(VglImage));
	startVgl(vgl);
	for(int i=0;;){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		if(i==0){
			copyIpl(vgl->ipl, frame);
			vgl->ndim=3;
			vgl->shape[0] = frame->width;
			vgl->shape[1] = frame->height;
			vgl->depth = frame->depth;
			vgl->nChannels = frame->nChannels;
			i++;
		}
		nframes++;
		addNDarray(frame->imageData, vgl, nframes);
	}
	vgl->shape[2] = nframes;
	VglInfo(vgl);
	cvWaitKey(0);
	cvReleaseImage(&frame);
	return vgl;
}
