void ImFlipH(Iplimage* img){
  int i, j, tam;
  char** imageData2;
  for(i=0;i<img->width;i++)
    for(j=0;j<img->height;j++)
      imageData2[img->width-i][img->height-j]=img->imageData[i+j];
  img->imageData = imageData2;
}
  
