#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<opencv2/core/types_c.h>
#include<math.h>



/*_____________________________________________________
  ________________________________________________________*/
IplImage* ImRead(char *fname){
  IplImage* img_out;
  FILE *fp;
  int w, h, i=0, type;
  if((fp = fopen(fname, "r")) == NULL){
    printf("\Erro na Abertura do Arquivo");
    return NULL;
  }
  fscanf(fp, "P%d", &type);

  //check for comments if exist
  //Ainda não suporta comentários...

  fscanf(fp, "%d %d\n", &w, &h);
  img_out = (IplImage*) malloc(sizeof(IplImage));
  unsigned int b, data;
  switch (type){
  case 1:
    img_out->imageData = (char*) malloc((1/8)*(w*h));
    for(;i<w*h;++i){
      fscanf(fp, "%d", &data);
      img_out->imageData[i] = data;
    }
    break;
 case 2:
   img_out->imageData = (char*) malloc(w*h);
   img_out->nChannels = 1;
   img_out->depth = IPL_DEPTH_8U;
   fscanf(fp, "%d\n", &b);
   for(;i<w*h;i++){
     fscanf(fp, "%d", &data);
     img_out->imageData[i]=data;
   }
   break;
  case 3:
    img_out->imageData = (char*) malloc(w*h*3);
    img_out->nChannels = 3;
    img_out->depth = IPL_DEPTH_8U;
    fscanf(fp, "%d\n", &b);
    for(;i<w*h*3;i++){
      fscanf(fp, "%d", &data);
      img_out->imageData[i]=data;
    }
    break;
  case 4:
    img_out->imageData = (char*) malloc((1/8)*(w*h));
    fread(img_out->imageData, ((1/8)*(w*h)), 1, fp);
    break;
  case 5:
    img_out->imageData=(char*) malloc(w*h);
    img_out->nChannels=1;
    img_out->depth=IPL_DEPTH_8U;
    fscanf(fp, "%d\n", &b) ;
    fread(img_out->imageData, w*h, 1, fp);
    break;
  case 6:
    img_out->imageData = (char*) malloc(w*h*3);
    img_out->nChannels = 3;
    img_out->depth=IPL_DEPTH_8U;
    fscanf(fp, "%d\n", &b);
    fread(img_out->imageData, w*h*3, 1, fp);
    break;
  default:
    printf("TIPO INVÁLIDO");
    exit(0);
    break;
  }
  img_out->ID = type;
  img_out->width=w;
  img_out->height=h;
  fclose(fp);
  return img_out;
}

void ImWrite(IplImage *img, char *fname){
  FILE *fp;
  if((fp=fopen(fname, "w"))==NULL){
    printf("\nERRO NA CRIAÇÃO DA IMAGEM");
    return;
  }
  switch (img->ID){
  case 1:
    fprintf(fp, "P1\n%d %d\n", img->width, img->height);
    break;
  case 2:
    fprintf(fp, "P2\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, img->depth)-1));
    break;
  case 3:
    fprintf(fp, "P3\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, img->depth)-1));
    break;
  case 4:
    fprintf(fp, "P4\n%d %d\n", img->width, img->height);
    break;
  case 5:
    fprintf(fp, "P5\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, img->depth)-1));
    break;
  case 6:
    fprintf(fp, "P6\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, img->depth)-1));
    break;
  default:
    printf("\nTIPO DE IMAGEM INVÁLIDA");
    exit(0);
    break;
  }
  int i=0;
  switch (img->ID){
  case 1:
  case 2:
    for(;i<(img->width*img->height);i++)
    {
      if(i > 0 && i % img->width == 0)
          fprintf(fp, "\n");
      fprintf(fp, "%u ", (unsigned char)img->imageData[i]);
    }
    fprintf(fp, "\b");
    break;
  case 3:
    for(;i<(img->width*img->height*3);i++)
    {
      if(i > 0 && i % img->width == 0)
          fprintf(fp, "\n");
      fprintf(fp, "%u ", (unsigned char)img->imageData[i]);
    }
    fprintf(fp, "\b");
    break;
  case 4:
    fwrite(img->imageData, ((1/8)*(img->width*img->height)), 1, fp);
    break;
  case 5:
    fwrite(img->imageData, (img->width*img->height), 1, fp);
    break;
  case 6:
    fwrite(img->imageData, (img->width*img->height*3), 1, fp);
    break;
  }
  fclose(fp);
}
void ImPrintData(char* imageData, int size)
{
    int i;
    for(i=0; i<size; i++)
      printf("%d ", (char)imageData[i]);
    printf("\n");

}
void ImPrintHeader(IplImage* img){
  printf("\n\nINICIANDO LEITURA DA IMAGEM");
  printf("\nID: %d", img->ID);
  printf("\ninChannels: %d", img->nChannels);
  printf("\ndepth: %d", img->depth);
  printf("\nwidth: %d", img->width);
  printf("\nheight %d", img->height);
  printf("\nsize of imgData %d", sizeof(img->imageData));
  printf("\ndeseja mostrar imageData?(yes | no) ");
  char yes[5] = "yes\0";
  char opc[255];
  scanf("%s", opc);
  printf("\n");
  if(strcmp(opc, yes)==0){

    printf("chk 10\n");
    ImPrintData(img->imageData, img->width*img->height);

  }
  else
    printf("\n\n");
}

void ImInvert(IplImage *img){
  int i=0;
  switch (img->ID){
  case 1:
    for(;i<img->width*img->height;i++){
      img->imageData[i]= 1 - img->imageData[i];
    }
    break;
  case 2:
    for(;i<img->width*img->height;i++){
      img->imageData[i]=((int)(pow(2, img->depth)-1))-img->imageData[i];
    }
    break;
  case 3:
    for(;i<img->width*img->height*3; i+=3){
      img->imageData[i]=((int)(pow(2, img->depth)-1))-img->imageData[i];
    }
    break;
  case 4:
  case 5:
    for(;i<img->width*img->height;i++)
      img->imageData[i]=(int)(pow(2, img->depth)-1)-(int)img->imageData[i];
    break;
  case 6:
    for(;i<img->width*img->height*3;i++)
      img->imageData[i]=(int)(pow(2, img->depth)-1)-(int)img->imageData[i];
    break;
  default:
   printf("\nFormato de Imagem Invalido");
   break;
  }
  return;
}

/*_________________________________________
::::::::::::::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::
::::::::::NOVAS FUNÇÕES:::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::
___________________________________________*/

void ImFlipV(IplImage* img){
  int i, j, p1, p2;
  uchar temp;
  switch(img->ID){
  case 1:
  case 2:
  case 4:
  case 5:
     for(i=0;i<(img->width/2);i++)
      for(j=0; j<(img->width); j++){
        p1 = i * img->width + j;
        p2 = (img->height - i - 1) * img->width + j;
	temp = img->imageData[p1];
	img->imageData[p1] = img->imageData[p2];
	img->imageData[p2] = temp;
      }
    break;
  case 3:
  case 6:
    break;
  }
}

void ImFlipH(IplImage* img){
  int p1, p2;
  uchar temp;
  switch(img->ID){
  case 1:
  case 2:
  case 4:
  case 5:
    for(int i=0;i<(img->width);i++)
      for(int j=0;j<(int)(img->height/2);j++){
	p1 = (i*img->height -1) + j;
	p2 = (i*img->height -1) + (img->height -j);
	temp = img->imageData[p1];
	img->imageData[p1] = img->imageData[p2];
	img->imageData[p2] = temp;
      }
    break;
  case 3:
  case 6:
    break;
  }
}

int main(int argc, char *argv[]){
  printf("\n");
  IplImage *img;
  img = ImRead(argv[1]); //argv[1] pega o nome de entrada da imagem
  if(argc > 3){
    int c=3;
    char color[9]="-icolor\0";
    char fliph[8]="-fliph\0";
    char flipv[9]="-flipv\0";
    for(;c<argc;c++){
      if(strcmp(color, argv[c])==0)
	ImInvert(img);
       else if(strcmp(fliph, argv[c])==0)
	 ImFlipH(img);
      else if(strcmp(argv[c], flipv)==0)
	ImFlipV(img);
    }
  }   
  ImWrite(img, argv[2]); //argv[2] pega o nome de saída da imagem
  printf("\nPrograma finalizado\n");
  return 0;
}
