#include "image.h"

IplImage *img_read(char *file_name){
	IplImage *img = (IplImage*) malloc(sizeof(IplImage));
	FILE *fp;
	if ((fp = fopen(file_name, "r")) == NULL){
		if (feof(fp)) printf("Premature end of file!");
		printf("Error while opening file!\n");
		return NULL;
	}
	fscanf(fp, "P%d", &img->ID);
	fscanf(fp, "%d %d\n", &img->width, &img->height);
	
	uint tmp;
	
	switch(img->ID){
		case 1: 
			DATA = (char*) malloc((1/8)*IMG_SIZE);
			for(int i=0; i<IMG_SIZE; i++){
				fscanf(fp, "%d",&tmp);
				DATA[i] = tmp;
			}
			break; //PBM ASCII
		case 2:
			DATA = (char*) malloc(IMG_SIZE);
			CHANNELS = 1;
			DEPTH = IPL_DEPTH_8U;
			fscanf(fp, "%d\n", &tmp);
			for (int i=0; i<IMG_SIZE; i++){
				fscanf(fp, "%d", &tmp);
				DATA[i] = tmp;
			}
			break; //PGM ASCII
		case 3:
			DATA = (char*) malloc(IMG_SIZE*3);
			CHANNELS = 3;
			DEPTH = IPL_DEPTH_8U;
			fscanf(fp, "%d", &tmp);
			for(int i=0; i<IMG_SIZE; i++){
				fscanf(fp, "%d", &tmp);
				DATA[i] = tmp;
			}
			break; //PPM ASCII
		case 4:
			DATA = (char*) malloc((1/8)*IMG_SIZE);
			fread(DATA, ((1/8)*IMG_SIZE), 1, fp);
			break; //PBM BIN
		case 5:
			DATA = (char*) malloc(IMG_SIZE);
			CHANNELS = 1;
			DEPTH = IPL_DEPTH_8U;
			fread(DATA, IMG_SIZE, 1, fp);
			break; //PGM BIN
		case 6:
			DATA = (char*) malloc(IMG_SIZE*3);
			CHANNELS = 3;
			DEPTH = IPL_DEPTH_8U;
			fread(DATA, IMG_SIZE*3, 1, fp);
			break; //PPM BIN
		default:
			printf("Invalid type!\n");
			exit(0);
			break;
	}
	fclose(fp);
	return img;
	
}

void print_console(IplImage *img){
	printf("\n\nReading image...");
	printf("\nID: %d", img->ID);
	printf("\nChannels: %d", CHANNELS);
	printf("\nDepth: %d", DEPTH);
	printf("\nWidth: %d", img->width);
	printf("\nHeight: %d", img->height);
	printf("\nSize of imageData: %d", sizeof(img->imageData));
	printf("\nShow image data? (y,N): ");
	char ch = getchar();
	printf("\n");
	if (ch == 'y')
		print_img_data(DATA, IMG_SIZE);
	else
		printf("\n");
}



void print_img_data(char *imageData, int size){
	for (int i=0; i<size; i++)
		printf("%d", (char)imageData[i]);
	printf("\n");
}

void img_write(IplImage *img, char *file_name){
	FILE *fp;
	if ((fp=fopen(file_name, "w"))==NULL){
		printf("\nCould not create image!");
		return;
	}
	switch (img->ID){
		case 1:
			fprintf(fp, "P1\n%d %d\n", img->width, img->height);
			break;
		case 2:
			fprintf(fp, "P2\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, DEPTH)-1));
			break;
		case 3:
			fprintf(fp, "P3\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, DEPTH)-1));
			break;
		case 4:
			fprintf(fp, "P4\n%d %d\n%d\n", img->width, img->height);
			break;
		case 5:
			fprintf(fp, "P5\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, DEPTH)-1));
			break;
		case 6:
			fprintf(fp, "P6\n%d %d\n%d\n", img->width, img->height, (int)(pow(2, DEPTH)-1));
			break;
	}
	
	switch (img->ID){
		case 1:
		case 2:
			for(int i=0; i<IMG_SIZE; i++){
				//Ao final da linha, pula-a
				if ( (i>0) && ((i % img->width) == 0) )
					fprintf(fp, "\n");
				fprintf(fp, "%u", (unsigned char)DATA[i]);
			}
			fprintf(fp, "\b");
			break;
		case 3:
			for(int i=0; i<IMG_SIZE; i++){
				if ( (i>0) && ((i % img->width) == 0) )
					fprintf(fp, "\n");
				fprintf(fp, "%u", (unsigned char)DATA[i]);
			}
			fprintf(fp, "\b");
			break;
		case 4:
			fwrite(DATA, ((1/8)*(IMG_SIZE)), 1, fp);
			break;
		case 5:
			fwrite(DATA, IMG_SIZE, 1, fp);
			break;
		case 6:
			fwrite(DATA, (IMG_SIZE*3), 1, fp);
			break;
	}
	fclose(fp);
}

void img_invert(IplImage *img) {
	switch (img->ID){
		case 1:
		case 4:
			for(int i=0; i < IMG_SIZE; i++)
				DATA[i] = 1 - DATA[i];
			break;
		case 2:
		case 5:
			for(int i=0; i < IMG_SIZE; i++)
				DATA[i] = DEPTH_IMG - DATA[i];
			break;
		case 3:
			for(int i=0; i < IMG_SIZE*3; i+=3)
				DATA[i] = DEPTH_IMG - DATA[i];
			break;
		case 6:
			for(int i=0; i < IMG_SIZE*3; i++)
				DATA[i] = DEPTH_IMG - (int)DATA[i];
			break;
	}
}

void img_threshold(IplImage *img, unsigned char n){
	for(int i=0; i < IMG_SIZE; i++){
		if( (unsigned char)DATA[i] >= n )
			DATA[i] = 255;
		else
			DATA[i] = 0;
	}
}

void img_flip_vert(IplImage *img){
	int k, z;
	unsigned char tmp;
	
	switch(img->ID){
		case 1:
		case 2:
		case 4:
		case 5: for(int i=0; i < (img->width)/2; i++)
				for(int j=0; j < (img->width); j++){
				k = i * img->width + j;
				z = (img->height - i - 1)*img->width +j;
				tmp = DATA[k];
				DATA[k] = DATA[z];
				DATA[z] = tmp;
			}
			break;
		case 3:
		case 6:
			break;
	}
}

int get_pos(IplImage *img, int x, int y){
	return ((x*img->width)+y);
}

int get_pos_ch(IplImage *img, int x, int y, int c){
	switch(c){
		case 1:
			return((3*x*img->width)+y);
		case 2:
			return((3*x*img->width)+(y+1));
		case 3:
			return((3*x*img->width)+(y+2));
		default:
			return -1;
	}
}


void img_flip_horiz(IplImage *img){
	int n;
	char tmp, d1, d2, d3;
	if ((img->height % 2) == 0)
		n = (img->height/2);
	else
		n = ((img->height - 1)/2);
	switch(img->ID){
		case 1:
		case 2:
		case 4:
		case 5:
			for (int i=0; i<img->width; i++)
				for(int j=0; j<n; j++){
					tmp = DATA[get_pos(img, i, j)];
					DATA[get_pos(img, i, j)] = DATA[get_pos(img, i, (img->height -j))];
					DATA[get_pos(img, i, (img->height -j))] = tmp;
				}
			break;
		case 3:
		case 6:
			for(int i=0; i<img->width; i++)
				for(int j=0; j<n; j++){
					d1 = DATA[get_pos_ch(img, i, j, 1)];
					d2 = DATA[get_pos_ch(img, i, j, 2)];
					d3 = DATA[get_pos_ch(img, i, j, 3)];
					
					DATA[get_pos_ch(img, i, j, 1)] = DATA[get_pos_ch(img, i, (img->height -j), 1)];
					DATA[get_pos_ch(img, i, j, 2)] = DATA[get_pos_ch(img, i, (img->height -j), 2)];
					DATA[get_pos_ch(img, i, j, 3)] = DATA[get_pos_ch(img, i, (img->height -j), 3)];
					
					DATA[get_pos_ch(img, i, (img->height -j), 1)] = d1;
					DATA[get_pos_ch(img, i, (img->height -j), 2)] = d2;
					DATA[get_pos_ch(img, i, (img->height -j), 3)] = d3;
				}
			break;
	}
}

void img_media(IplImage *img){
	unsigned char *pix = (unsigned char *) malloc (sizeof(char)*IMG_SIZE);
	unsigned char val;
	
	for (int i=0; i<IMG_SIZE; i++)
		pix[i] = DATA[i];

	for (int i=1; i < (img->width -1); i++)
		for(int j=1; j < (img->height -1); j++){
			val = (
				(pix[get_pos(img, i-1, j-1)] / 9) +
				(pix[get_pos(img, i-1, j  )] / 9) +
				(pix[get_pos(img, i-1, j+1)] / 9) +
				(pix[get_pos(img, i  , j-1)] / 9) +
				(pix[get_pos(img, i  , j  )] / 9) +
				(pix[get_pos(img, i  , j+1)] / 9) +
				(pix[get_pos(img, i+1, j-1)] / 9) +
				(pix[get_pos(img, i+1, j  )] / 9) +
				(pix[get_pos(img, i+1, j+1)] / 9)
			);
			DATA[get_pos(img, i, j)] = val;
		}
	free(pix);
}

void img_blur(IplImage *img){
	unsigned char *pix = (unsigned char*) malloc (sizeof(char)*IMG_SIZE);
	unsigned char val;
	
	for (int i=0; i < IMG_SIZE; i++)
		pix[i] = DATA[i];
	
		for (int i=1; i < (img->width -1); i++)
		for (int j=1; j < (img->height -1); j++){
		val = (
			(pix[get_pos(img, i-1, j-1)]*1)/16 +
			(pix[get_pos(img, i-1, j  )]*2)/16 +
			(pix[get_pos(img, i-1, j+1)]*1)/16 +
			(pix[get_pos(img, i  , j-1)]*2)/16 +
			(pix[get_pos(img, i  , j  )]*4)/16 +
			(pix[get_pos(img, i  , j+1)]*2)/16 +
			(pix[get_pos(img, i+1, j-1)]*1)/16 +
			(pix[get_pos(img, i+1, j  )]*2)/16 +
			(pix[get_pos(img, i+1, j+1)]*1)/16
		);
		DATA[get_pos(img, i, j)] = val;
	}free(pix);
}

void img_laplacian(IplImage *img){
	unsigned char *pix = (unsigned char*) malloc (sizeof(char)*IMG_SIZE);
	unsigned char val;
	
	for (int i=0; i < IMG_SIZE; i++)
		pix[i] = DATA[i];
	
	for (int i=1; i < (img->width -1); i++)
		for (int j=1; j < (img->height -1); j++){
		val = (
			(pix[get_pos(img, i-1, j-1)]*0) +
			(pix[get_pos(img, i-1, j  )]*(-1)) +
			(pix[get_pos(img, i-1, j+1)]*0) +
			(pix[get_pos(img, i  , j-1)]*(-1))+
			(pix[get_pos(img, i  , j  )]*4) +
			(pix[get_pos(img, i  , j+1)]*(-1)) +
			(pix[get_pos(img, i+1, j-1)]*0) +
			(pix[get_pos(img, i+1, j  )]*(-1)) +
			(pix[get_pos(img, i+1, j+1)]*0)
		);
		DATA[get_pos(img, i, j)] = val;
	}free(pix);
}

