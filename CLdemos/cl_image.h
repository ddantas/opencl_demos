#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include "image.h"

typedef struct CL {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
} CL;

const char** getKernelPtr(const char*);

void StartPlatform(CL*);
void StartDevice(CL*);
void StartContext(CL*);
void StartQueue(CL*);
void ReleaseCL(CL*);
void StartCL(CL*);
void cl_ImInvert(CL*, IplImage*);
void cl_ImThreshold(CL*, IplImage*, unsigned char);


void cl_ImInvert2(CL*, IplImage*);



