#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<opencv2/core/types_c.h>
#include<math.h>

IplImage* ImRead(char *fname);
void ImWrite(IplImage *img, char *fname);
void ImPrintData(char* imageData, int size);
void ImPrintHeader(IplImage* img);
void ImInvert(IplImage *img);
void ImFlipV(IplImage* img);
void ImFlipH(IplImage* img);
void ImThreshold(IplImage* img, unsigned char t);
void ImFilMed(IplImage* img);
void ImBlur(IplImage* img);
void ImLaplac(IplImage* img);
