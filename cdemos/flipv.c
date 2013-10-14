void flipv(IplImage *img){
  int i, j;
  unsigned char *data[];
  for(i=0;i<img->width;i++)
    for(j=0;j<img->height;j++)
      data[i][j]=img->imageData[i][j];
  for(i=0, j=1;i<(img->width*img->height);i++)
    data[j][i]=img->imageData[i];
