#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <opencv2/core/types_c.h>
#define MAX_DIM 3
/*	Structure of 3D image */
typedef struct _VglImage {
	IplImage* img;
	char* ndarray;
	int shape[MAX_DIM];
} VglImage;
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
