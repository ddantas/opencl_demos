#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
#include "image.h"


typedef struct CL{
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
} CL;

const char **getKernelPtr(const char*);
void startPlatform(CL *);
void startDevice(CL *);
void startContext(CL *);
void startQueue(CL *);
void startCL(CL *);
void releaseCL(CL *);
void cl_invert(CL *, IplImage *);
void cl_threshold(CL *, IplImage *, unsigned char n);
void cl_invert_2d(CL *, IplImage *);