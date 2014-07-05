#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <cv.h>
#include <highgui.h>
/*VglImage - BEGIN*/
#define VGL_MAX_DIM 3
#define VGL_NFRAMES_MAX 1024
typedef struct _VglImage {
	IplImage* ipl;
	char* ndarray;
	int ndim;
	int shape[VGL_MAX_DIM];
	int depth;
	int nChannels;
} VglImage;
void rgb2rgba(IplImage*, VglImage*);
//VglImage* startVgl();
//void copyIpl(IplImage*, IplImage*);
//void addNDarray(char*, VglImage*, int);
//void VglInfo(VglImage*);
VglImage* getVglImage(const char*);
/*END*/
/*	Structure of the base type CL */
//	Type definition
typedef struct CL {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
} CL;
//	Function to obtain kernel
const char** getKernelPtr(const char*);
// Initialization functions of structure CL
void StartPlatform(CL*);
void StartDevice(CL*);
void StartContext(CL*);
void StartQueue(CL*);
void StartCL(CL*);
//	Release functions of structure CL
void ReleaseCL(CL*);
/*	Test of wrapper functions */
//	Invert a image like a array buffer
void clInvertA(CL*, IplImage*);
//	Invert a image of 2 dimensions 
void clInvert2D(CL*, IplImage*);
//	Invert a image of 3 dimensions
void clInvert3D(CL*, VglImage*);
