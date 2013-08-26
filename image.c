
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core/types_c.h>


void PrintImageHeader(IplImage* img_in, char* msg)
{
  printf("%s\n", msg);
  printf("Size: (w, h) = (%d, %d)\n", img_in->width, img_in->height);
  printf("nChannels: %d\n", img_in->nChannels);
  printf("depth: %d\n", img_in->depth);
  printf("imageData: %p\n", img_in->imageData);
}

void PrintImageData(IplImage* img_in, char* msg)
{
  printf("%s\n", msg);
  int w = img_in->width;
  int h = img_in->height;
  int n = img_in->nChannels;
  for (int i = 0; i < h; i++){
      printf("%d: ", i);
      for (int j = 0; j < w; j++){
          for (int k = 0; k < img_in->nChannels; k++){
              printf("%2x ", img_in->imageData[i*w*n + j*n + k]  & 0xff);
	  }
      }
      printf("\n");
  }
}



IplImage* imread(char* filename){
    IplImage* img_out;
    FILE *fp;
    int w, h;
    char p;
    int key;

    fp = fopen(filename, "r"); //abrindo a imagem apenas para leitura, para garantir que a imagem original não será alterada

    fscanf(fp, "%c%d", &p, &key);

    printf("%c%d\n", p, key);

    //check for comments
    char c = getc(fp);
    while (c == '#' || c == 10)
    {
        while (getc(fp) != '\n')
	{
	}
        c = getc(fp);
    }
    ungetc(c, fp);
  

    fscanf(fp, "%d %d", &w, &h);
    printf("%d %d\n", w, h);

    c = getc(fp);
    while (c == '#' || c == 10)
    {
        while (getc(fp) != '\n')
	{
	}
        c = getc(fp);
    }
    ungetc(c, fp);

    printf("End of header\n");

    img_out = (IplImage*) malloc(sizeof(IplImage));

    switch (key) {
		case 1:  //Se for P1, no formato .pbm (ASCII)
		  img_out->imageData = malloc(w*h/8+1);
                  fread(img_out->imageData, w*h/8+1, 1, fp);
		  break;
		case 2:  //Se for P2 no formato .pgm (ASCII)
		  img_out->imageData = malloc(w*h);
                  img_out->nChannels = 1;
                  img_out->depth = IPL_DEPTH_8U;
                  fread(img_out->imageData, w*h, 1, fp);
		  break;
		case 3:  //Se for P3 no formato .ppm (ASCII)
		  img_out->imageData = malloc(w*h*3);
                  img_out->nChannels = 3;
                  img_out->depth = IPL_DEPTH_8U;
                  fread(img_out->imageData, w*h*3, 1, fp);
		  break;
		case 4: //Se for P4 no formato .pbm (bin)
		  img_out->imageData = malloc(w*h/8+1);
                  fread(img_out->imageData, w*h/8+1, 1, fp);
		  break;
		case 5: 
		  img_out->imageData = malloc(w*h);
                  img_out->nChannels = 1;
                  img_out->depth = IPL_DEPTH_8U;
                  fread(img_out->imageData, w*h, 1, fp);
		  break;
		case 6: 
		  img_out->imageData = malloc(w*h*3);
                  img_out->nChannels = 3;
                  img_out->depth = IPL_DEPTH_8U;
                  fread(img_out->imageData, w*h*3, 1, fp);
		  break;
	}
        img_out->width = w;
        img_out->height = h;
	fclose(fp);
        return img_out;
}


/*
int main()
{
  IplImage* img = imread("images/lena512color.ppm");
  PrintImageData(img, "Reading lena image"); 
}
*/
