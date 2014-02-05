#include "cl_image.h"

const char** getKernelPtr(const char* name){
	FILE *program_handle;
	char **program_buffer;
	size_t program_size;
	
	program_handle = fopen(name, "r");
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	
	program_buffer = (char**)malloc(sizeof(char));
	program_buffer[0] = (char*)malloc(program_size + 1);
	program_buffer[0][program_size] = '\0';
	fread(program_buffer[0], sizeof(char), program_size, program_handle);
	
	fclose(program_handle);
	
	return (const char**)program_buffer;
}


void cl_error(int error);

void cl_ImInvert2(CL* cl, IplImage* img){
	cl_program program;
	cl_kernel kernel;
	cl_int err;
	
	cl_mem mobj_A = NULL;
	
	cl_image_format format;
	format.image_channel_order = CL_A;
	format.image_channel_data_type = CL_UNORM_INT8;

	mobj_A = clCreateImage2D(cl->context, CL_MEM_READ_ONLY, &format, img->width, img->height, NULL, NULL, &err);
	printf("IMAGE STATUS: ");
	cl_error(err);
}

void cl_ImThreshold(CL* cl, IplImage* img, unsigned char thre){
	cl_program program;
	cl_kernel kernel;
	//cl_mem bufA;
	cl_mem bufB;
	cl_mem bufC;
	cl_int err;
	int tam = img->width*img->height;
	
	char* hostC = (char*)malloc(tam*sizeof(char));
	
	size_t globalSize[1] = { tam };
	
	//const char* k = "./CL/Threshold.cl";
	const char* k = "./CLdemos/CL/Threshold.cl";
	const char** fonte = getKernelPtr(k);
	
	program = clCreateProgramWithSource(cl->context, 1, fonte, NULL, &err);
	
	clBuildProgram(program, 0, NULL, NULL, NULL, &err);
	printf("BUILD STATUS: "); cl_error(err);
	
	kernel = clCreateKernel(program, "Threshold", &err);
	printf("KERNEL    "); cl_error(err);
	
	//bufA = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(char), NULL, &err);
	//printf("BUF A STATUS: "); cl_error(err);
	bufB = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, (tam*sizeof(char)), NULL, &err);
	printf("BUF B STATUS: "); cl_error(err);
	bufC = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, (tam*sizeof(char)), NULL, &err);
	printf("BUF C STATUS: "); cl_error(err);
	//clEnqueueWriteBuffer(cl->queue, bufA, CL_TRUE, 0, sizeof(char), &thre, 0, NULL, NULL);
	clEnqueueWriteBuffer(cl->queue, bufB, CL_TRUE, 0, tam*sizeof(char), img->imageData, 0, NULL, NULL);
	//clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufB);
	//clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufC);
	//clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufA);
	
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufC);
	
	clEnqueueNDRangeKernel(cl->queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	
	clFinish(cl->queue);
	
	clEnqueueReadBuffer(cl->queue, bufC, CL_TRUE, 0, tam*sizeof(char), hostC, 0, NULL, NULL);
	printf("ENQUEUE BUFFER STATUS: "); cl_error(err);
	
	for(int i=0; i<tam; i++)
		img->imageData[i] = hostC[i];

	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
}

void cl_ImInvert(CL* cl, IplImage* img){
	cl_program program;
	cl_kernel kernel;
	cl_mem bufB;
	cl_mem bufC;
	cl_int err;
	int tam = img->width*img->height;
	
	char* hostC = (char*)malloc(tam*sizeof(char));
	
	size_t globalSize[1] = { tam };
	
	//const char* k = "./CL/Invert.cl";
	const char* k = "./CLdemos/CL/Invert.cl";
	const char** fonte = getKernelPtr(k);
	
	program = clCreateProgramWithSource(cl->context, 1, fonte, NULL, &err);
	printf("PROGRAM STATUS: "); cl_error(err);
	
	clBuildProgram(program, 0, NULL, NULL, NULL, &err);
	printf("BUILD STATUS: "); cl_error(err);
	
	kernel = clCreateKernel(program, "Invert", &err);
	printf("KERNEL    "); cl_error(err);
	
	
	bufB = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, (tam*sizeof(char)), NULL, &err);
	printf("BUF B STATUS: "); cl_error(err);
	bufC = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, (tam*sizeof(char)), NULL, &err);
	printf("BUF C STATUS: "); cl_error(err);
	clEnqueueWriteBuffer(cl->queue, bufB, CL_TRUE, 0, tam*sizeof(char), img->imageData, 0, NULL, NULL);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufC);
	
	clEnqueueNDRangeKernel(cl->queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	
	clFinish(cl->queue);
	
	clEnqueueReadBuffer(cl->queue, bufC, CL_TRUE, 0, tam*sizeof(char), hostC, 0, NULL, NULL);
	printf("ENQUEUE BUFFER STATUS: "); cl_error(err);
	
	for(int i=0; i<tam; i++)
		img->imageData[i] = hostC[i];

	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
}





void StartPlatform(CL* cl){
	cl_int err;
	char* info;
	size_t size;
	
	err = clGetPlatformIDs(1, &cl->platform, NULL);
	printf("PLATFORM STATUS: "); cl_error(err);
	if(err != 0)
		return;
	
	clGetPlatformInfo(cl->platform, CL_PLATFORM_NAME, 0, NULL, &size);
	info=(char*)malloc(size*sizeof(char));
	clGetPlatformInfo(cl->platform, CL_PLATFORM_NAME, size, info, NULL);
	printf("USING PLATFORM %s\n", info);
	printf("\n\n");
	free(info);
}

void StartDevice(CL* cl){
	cl_int err;
	char* info;
	size_t size;
	
	err = 
	clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_GPU, 1, &cl->device, NULL);
	if(err != 0){
		printf("Nenhuma GPU encontrada...\n");
		err =
		clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_CPU, 1, &cl->device, NULL);
	}
	printf("DEVICE STATUS: "); cl_error(err);
	clGetDeviceInfo(cl->device, CL_DEVICE_NAME, 0, NULL, &size);
	info=(char*)malloc(size*sizeof(char));
	clGetDeviceInfo(cl->device, CL_DEVICE_NAME, size, info, NULL);
	
	printf("USING DEVICE: %s\n", info);
	
	free(info);
	printf("\n\n");
}

void StartContext(CL* cl){
	cl_int err;
	cl->context = clCreateContext(0, 1, &cl->device, NULL, NULL, &err);
	printf("CONTEXT STATUS: "); cl_error(err);
	printf("\n\n");
}

void StartQueue(CL* cl){
	cl_int err;
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &err);
	printf("QUEUE STATUS: "); cl_error(err);
	printf("\n\n");
}

void ReleaseCL(CL* cl){
	clReleaseCommandQueue(cl->queue);
	clReleaseContext(cl->context);
}

void StartCL(CL* cl){
	StartPlatform(cl);
	StartDevice(cl);
	StartContext(cl);
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
