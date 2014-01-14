#include "image.h"


int main(int argc, char** argv) {
  IplImage *img = ImRead(argv[1]);
  int v;
  printf("DIGITE UM NUMERO ");
  scanf("%d", &v);
  printf("\n");
  char c=0;
  if((v<=255)&&(v>=0))
	c = (char)v;
  ImThreshold(img, c);
  ImWrite(img, argv[2]);
  return 0;
}
