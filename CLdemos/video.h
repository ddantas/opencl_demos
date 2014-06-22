#define MAX_DIM 3
/*	Structure of 3D image */
typedef struct _VglImage {
	IplImage* ipl;
	char* ndarray;
	int ndim;
	int shape[MAX_DIM];
	int depth;
	int nChannels;
} VglImage;

void copyIpl(IplImage*, IplImage*);
void addNDarray(char*, VglImage*, int);
VglImage* getVglImage(const char*);
