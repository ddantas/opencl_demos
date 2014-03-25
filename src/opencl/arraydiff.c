#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>

#define ARRAY_LENGTH 1000000

int main(int argc, char** argv){
	
	cl_platform_id platformId;
	cl_device_id deviceId;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_event event;
	cl_mem bufA;
	cl_mem bufB;
	cl_mem bufC;
	
	int* hostA;
	int* hostB;
	int* hostC;
	int i;
	size_t globalSize[1] = { ARRAY_LENGTH };
	
	const char* source =
	"__kernel void ArrayDiff( \
		__global const int* a, \
		__global const int* b, \
		__global int* c) \
		{ \
			int id = get_global_id(0); \
			c[id] = a[id] - b[id]; \
		}";
	
	clGetPlatformIDs(1, &platformId, NULL);
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, NULL);
	context = clCreateContext(0, 1, &deviceId, NULL, NULL, NULL);
	queue = clCreateCommandQueue(context, deviceId, 0, NULL);
	program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "ArrayDiff", NULL);
	
	hostA = (int*) malloc (ARRAY_LENGTH * sizeof(int));
	hostB = (int*) malloc (ARRAY_LENGTH * sizeof(int));
	hostC = (int*) malloc (ARRAY_LENGTH * sizeof(int));
	
	for(i=0; i<ARRAY_LENGTH; i++){
		hostA[i] = rand() % 101 - 50;
		hostB[i] = rand() % 101 - 50;
	}
	
	bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_LENGTH * sizeof(int), NULL, NULL);
	bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_LENGTH * sizeof(int), NULL, NULL);
	bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_LENGTH * sizeof(int), NULL, NULL);
	
	clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, ARRAY_LENGTH * sizeof(int), hostA, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, ARRAY_LENGTH * sizeof(int), hostB, 0, NULL, NULL);
	
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
	
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	clFinish(queue);
	clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, ARRAY_LENGTH * sizeof(int), hostC, 0, NULL, NULL);
	
	for(i=0; i<ARRAY_LENGTH; i++)
		printf("%d - %d = %d\n", hostA[i], hostB[i], hostC[i]);
	
	clReleaseMemObject(bufA);
	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	free(hostA);
	free(hostB);
	free(hostC);
	
	return 0;
}