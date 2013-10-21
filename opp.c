#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/types_c.h>
#include <math.h>

#ifdef __APPLE__
 #include <OpenCL/opencl.h>
#else
 #include <CL/opencl.h>
#endif

#define TAM img->width*img->height*3

IplImage* iread(char* fname){
  IplImage* img_out;
  FILE *fp;
  if((fp = fopen(fname, "r")) == NULL){
    printf("/nERRO AO TENTAR ABRIR ARQUIVO");
    return NULL;
  }
  fscanf(fp, "P%d", &img_out->ID);
  //Parte para checar comentários
  int dep;
  fscanf(fp,"%d %d\n%d\n", &img_out->width, &img_out->height, &dep);
  img_out->imageData=(char*) malloc(img_out->width*img_out->height*3);
  img_out->nChannels = 3;
  img_out->depth = IPL_DEPTH_8U;
  fread(img_out->imageData, img_out->width*img_out->height*3, 1, fp);
  fclose(fp);
  return img_out;
}
void iwrite(IplImage *img, char *fname){
  FILE *fp;
  fp = fopen(fname, "w");
  fprintf(fp, "P6\n%d %d\n%d\n", img->width, img->height,(int)(pow(2, img->depth)-1));
  fwrite(img->imageData, (img->width*img->height*3), 1, fp);
  fclose(fp);
}
int main(int argc, char** argv){
  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_context context;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;
  cl_mem bufA;
  cl_mem bufB;
  
  IplImage* img;
  img = iread(argv[1]);
  int* hostA;
  hostA = (int*) malloc(TAM);
  hostA= (int*) img->imageData;
  int hostB = 255;
  size_t globalSize[1] = {TAM};
  int i;

  const char* Fkernel =
    "__kernel void InvertColor(__global int* a,\
__global const int depth){\
int id = get_global_id(0);\
a[id]=depth - (int)a[id];\
}";

  clGetPlatformIDs(1, &platformId, NULL);
  clGetDeviceIDs(platformId, CL_DEVICE_TYPE_CPU, 1, &deviceId, NULL);
  context = clCreateContext(0, 1, &deviceId, NULL, NULL, NULL);
  queue = clCreateCommandQueue(context, deviceId, 0, NULL);
  program = clCreateProgramWithSource(context, 1, &Fkernel, NULL, NULL);
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  kernel = clCreateKernel(program, "InvertColor", NULL);
  bufA = clCreateBuffer(context, CL_MEM_READ_WRITE, TAM, NULL, NULL);
  bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, TAM, NULL, NULL);
  clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, TAM, hostA, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, sizeof(int), &hostB, 0, NULL, NULL);
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
  clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
  clFinish(queue);
  clEnqueueReadBuffer(queue, bufA, CL_TRUE, 0, TAM, hostA, 0, NULL, NULL);
  
  iwrite(img, argv[2]);
  
  clReleaseMemObject(bufA);
  clReleaseMemObject(bufB);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
  free(hostA);
  return 0;
}
