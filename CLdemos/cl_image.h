#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#ifndef __OPENCV_OLD_CV_H__
 #include "image.h"
#endif

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
