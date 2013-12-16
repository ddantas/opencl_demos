#include <stdio.h>
#include <stdlib.h>
#include "invert.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

void clinvert(char *hostA, unsigned char hostB, int tam){
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
	
	cl_int errorcode;
	
	char* hostC;
	hostC = (char*) malloc(tam);
	printf("Criação das variáveis do OpenCL ativos\n");
	
	size_t globalSize[1] = { tam };
	
	const char* source =
	"__kernel void a\
	(__global const char* a, \
	__global const char b, \
	__global char* c){\
		int id = get_global_id(0);\
		c[id] = a[id];\
	}";
	
	printf("GetPlatform...\n");
	clGetPlatformIDs(1, &platformId, NULL);
	printf("GetDeviceIDs...\n");
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_CPU, 1, &deviceId, NULL);
	printf("GetPlatform ok...\nGetDevice ok...\n");
	
	printf("Context...\n");
	context = clCreateContext(0, 1, &deviceId, NULL, NULL, &errorcode);
	cl_error(errorcode);
	
	printf("Command Queue...\n");
	queue = clCreateCommandQueue(context, deviceId, 0, &errorcode);
	cl_error(errorcode);
	
	printf("Create Program...\n");
	program = clCreateProgramWithSource(context, 1, &source, NULL, &errorcode);
	cl_error(errorcode);
	printf("Build Program...\n");
	clBuildProgram(program, 0, NULL, NULL, NULL, &errorcode);
	cl_error(errorcode);
	
	printf("Create Kernel...\n");
	kernel = clCreateKernel(program, "a", &errorcode);
	cl_error(errorcode);
	
	printf("Buff Alocation...\n");
	bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, tam, NULL, &errorcode);
	cl_error(errorcode);
	printf("Buff Alocation...\n");
	bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(char), NULL, &errorcode);
	cl_error(errorcode);
	printf("Buff Alocation...\n");
	bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, tam, NULL, &errorcode);
	cl_error(errorcode);
	
	printf("Send Buff...\n");
	clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, tam, hostA, 0, NULL, NULL);
	cl_error(errorcode);
	printf("Send Buff...\n");
	clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, sizeof(char), &hostB, 0, NULL, NULL);
	cl_error(errorcode);
	
	printf("Set Kernel args...\n");
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
	printf("Completed...\n");
	
	printf("Enqueue Range Kernel...\n");
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	cl_error(errorcode);

	clFinish(queue);
	printf("Finish queue\n");

	clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, tam, hostC, 0, NULL, NULL);
	printf("Leitura do buff\n");
	
	for(int i=0; i<tam;i++)
		hostA[i] = hostC[i];
	
	clReleaseMemObject(bufA);
	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	free(hostC);
}

void cl_error(int error){
	switch(error){
		case 0:
			printf("CL_SUCCESS\n");
			break;
		case -1:
			printf("CL_DEVICE_NOT_FOUND\n");
			break;
		case -2:
			printf("CL_DEVICE_NOT_AVAILABLE\n");
			break;
		case -3:
			printf("CL_COMPILER_NOT_AVAILABLE\n");
			break;
		case -4:
			printf("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
			break;
		case -5:
			printf("CL_OUT_OF_RESOURCES\n");
			break;
		case -6:
			printf("CL_OUT_OF_HOST_MEMORY\n");
			break;
		case -7:
			printf("CL_PROFILING_INFO_NOT_AVAILABLE\n");
			break;
		case -8:
			printf("CL_MEM_COPY_OVERLAP\n");
			break;
		case -9:
			printf("CL_IMAGE_FORMAT_MISMATCH\n");
			break;
		case -10:
			printf("CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
			break;
		case -11:
			printf("CL_BUILD_PROGRAM_FAILURE\n");
			break;
		case -12:
			printf("CL_MAP_FAILURE\n");
			break;
		case -13:
			printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
			break;
		case -14:
			printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
			break;
		case -15:
			printf("CL_COMPILE_PROGRAM_FAILURE\n");
			break;
		case -16:
			printf("CL_LINKER_NOT_AVAILABLE\n");
			break;
		case -17:
			printf("CL_LINK_PROGRAM_FAILURE\n");
			break;
		case -18:
			printf("CL_DEVICE_PARTITION_FAILED\n");
			break;
		case -19:
			printf("CL_KERNEL_ARG_INFO_NOT_AVAILABLE\n");
			break;
		case -30:
			printf("CL_INVALID_VALUE\n");
			break;
		case -31:
			printf("CL_INVALID_DEVICE_TYPE\n");
			break;
		case -32:
			printf("CL_INVALID_PLATFORM\n");
			break;
		case -33:
			printf("CL_INVALID_DEVICE\n");
			break;
		case -34:
			printf("CL_INVALID_CONTEXT\n");
			break;
		case -35:
			printf("CL_INVALID_QUEUE_PROPERTIES\n");
			break;
		case -36:
			printf("CL_INVALID_COMMAND_QUEUE\n");
			break;
		case -37:
			printf("CL_INVALID_HOST_PTR\n");
			break;
		case -38:
			printf("CL_INVALID_MEM_OBJECT\n");
			break;
		case -39:
			printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
			break;
		case -40:
			printf("CL_INVALID_IMAGE_SIZE\n");
			break;
		case -41:
			printf("CL_INVALID_SAMPLER\n");
			break;
		case -42:
			printf("CL_INVALID_BINARY\n");
			break;
		case -43:
			printf("CL_INVALID_BUILD_OPTIONS\n");
			break;
		case -44:
			printf("CL_INVALID_PROGRAM\n");
			break;
		case -45:
			printf("CL_INVALID_PROGRAM_EXECUTABLE\n");
			break;
		case -46:
			printf("CL_INVALID_KERNEL_NAME\n");
			break;
		case -47:
			printf("CL_INVALID_KERNEL_DEFINITION\n");
			break;
		case -48:
			printf("CL_INVALID_KERNEL\n");
			break;
		case -49:
			printf("CL_INVALID_ARG_INDEX\n");
			break;
		case -50:
			printf("CL_INVALID_ARG_VALUE\n");
			break;
		case -51:
			printf("CL_INVALID_ARG_SIZE\n");
			break;
		case -52:
			printf("CL_INVALID_KERNEL_ARGS\n");
			break;
		case -53:
			printf("CL_INVALID_WORK_DIMENSION\n");
			break;
		case -54:
			printf("CL_INVALID_WORK_GROUP_SIZE\n");
			break;
		case -55:
			printf("CL_INVALID_WORK_ITEM_SIZE\n");
			break;
		case -56:
			printf("CL_INVALID_GLOBAL_OFFSET\n");
			break;
		case -57:
			printf("CL_INVALID_EVENT_WAIT_LIST\n");
			break;
		case -58:
			printf("CL_INVALID_EVENT\n");
			break;
		case -59:
			printf("CL_INVALID_OPERATION\n");
			break;
		case -60:
			printf("CL_INVALID_GL_OBJECT\n");
			break;
		case -61:
			printf("CL_INVALID_BUFFER_SIZE\n");
			break;
		case -62:
			printf("CL_INVALID_MIP_LEVEL\n");
			break;
		case -63:
			printf("CL_INVALID_GLOBAL_WORK_SIZE\n");
			break;
		case -64:
			printf("CL_INVALID_PROPERTY\n");
			break;
		case -65:
			printf("CL_INVALID_IMAGE_DESCRIPTOR\n");
			break;
		case -66:
			printf("CL_INVALID_COMPILER_OPTIONS\n");
			break;
		case -67:
			printf("CL_INVALID_LINKER_OPTIONS\n");
			break;
		case -68:
			printf("CL_INVALID_DEVICE_PARTITION_COUNT\n");
			break;
		default:
			printf("NO VALID CL ERROR CODE\n");
	}
}

//gcc -c Invert.c -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86 -lOpenCL -o invert.o -std=gnu99
