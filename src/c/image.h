#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/core/types_c.h>

#define IMG_SIZE img->width*img->height
#define DATA img->imageData
#define CHANNELS img->nChannels
#define DEPTH img->depth
#define DEPTH_IMG ((int)(pow(2, DEPTH)-1))

IplImage *img_read(char *file_name);
void print_console(IplImage *img);
void print_img_data(char *imageData, int size);
void img_write(IplImage *img, char *file_name);
void img_invert(IplImage *img);
void img_threshold(IplImage *img, unsigned char n);
void img_flip_vert(IplImage *img);
void img_flip_horiz(IplImage *img);
void img_media(IplImage *img);
void img_blur(IplImage *img);
void img_laplacian(IplImage *img);