#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/types_c.h>
#include <math.h>
#include "cdemos/libimage.h"

#ifdef __APPLE__
 #include <OpenCL/opencl.h>
#else
 #include <CL/opencl.h>
#endif

#define TAM img->width*img->height*3

IplImage* openIcolor(IplImage* img, int hostB){
  unsigned int* hostA;
  hostA = (unsigned int*) malloc(TAM);
  
  //Verificações...
  printf("\nsizeof hostA = %d\nsizeof imageData = %d", sizeof(hostA), sizeof(img->imageData));

  int i;
  for(i=0;i<TAM;i++)
   hostA[i] = img->imageData[i];
  for(i=0;i<=10;i++)
    printf("\nhostA[%d]=%d  imageData=%d", i, hostA[i], (int)img->imageData[i]);

  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_context context;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;
  cl_mem bufA;
  cl_mem bufB;
  size_t globalSize[1] = {TAM};
  const char* Fkernel = 
    "__kernel void InvertColor(__global int* a, \
__global const int depth){\
int id = get_global_id(0);\
a[id] = depth - a[id];\
}";
  clGetPlatformIDs(1, &platformId, NULL);
  clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, NULL);
  context = clCreateContext(0, 1, &deviceId, NULL, NULL, NULL);
  queue   = clCreateCommandQueue(context, deviceId, 0, NULL);
  program = clCreateProgramWithSource(context, 1, &Fkernel, NULL, NULL);
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  kernel  = clCreateKernel(program, "InvertColor", NULL);
  bufA    = clCreateBuffer(context, CL_MEM_READ_WRITE, TAM, NULL, NULL);
  bufB    = clCreateBuffer(context, CL_MEM_READ_WRITE, TAM, NULL, NULL);
  clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, TAM, hostA, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, sizeof(int), &hostB, 0, NULL, NULL);
  clSetKernelArg(kernel, 0, TAM, &bufA);
  clSetKernelArg(kernel, 1, sizeof(int), &bufB);
  clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
  clFinish(queue);
  clEnqueueReadBuffer(queue, bufA, CL_TRUE, 0, TAM, hostA, 0, NULL, NULL);

  for(i=0;i<TAM;i++)
    img->imageData[i] = (unsigned char) hostA[i];
  clReleaseMemObject(bufA);
  clReleaseMemObject(bufB);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
  free(hostA);
  return img;
}
int main(int argc, char** argv){  
  IplImage* img;
  img = iread(argv[1]);
  printf("\niread sucess");
  int depth = (int) (pow(2, img->depth)-1);
  if(!(argv[3] == "-icolor\0")){
    img = openIcolor(img, depth);
    printf("\nopenCL sucess");
  }
  iwrite(img, argv[2]);
  printf("\niwrite sucess\n");
  return 0;
}
