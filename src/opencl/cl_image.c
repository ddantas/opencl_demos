#include "cl_image.h"

const char** getKernelPtr(const char *name){
	FILE *fp;
	char **buffer;
	size_t size;
	
	fp = fopen(name, "r");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	buffer = (char**) malloc (sizeof(char));
	buffer[0] = (char*) malloc (size + 1);
	buffer[0][size] = '\0';
	fread(buffer[0], sizeof(char), size, fp);
	fclose(fp);
	
	return (const char **)buffer;
}

void cl_error(int error);

cl_image_desc getDesc(CL *cl, IplImage *img){
	cl_int err;
	cl_mem imageData = (cl_mem) malloc (IMG_SIZE);
	imageData = clCreateBuffer(cl->context, CL_MEM_COPY_HOST_PTR, IMG_SIZE, DATA, &err);
	printf("Buffer image status: "); cl_error(err);
	cl_image_desc image_d;
	image_d.image_type = CL_MEM_OBJECT_IMAGE2D;
	image_d.image_width = (size_t)img->width;
	image_d.image_height = (size_t)img->height;
	image_d.image_depth = (size_t)img->depth;
	image_d.image_array_size = (size_t)(IMG_SIZE);
	image_d.image_row_pitch = (size_t)img->width;
	image_d.image_slice_pitch = image_d.image_array_size;
	image_d.num_mip_levels = 0;
	image_d.num_samples = 0;
	image_d.buffer = imageData;
	
	return image_d;
}

void startPlatform(CL *cl){
	cl_int err;
	size_t size;
	char *info = (char*) malloc (size*sizeof(char));
	
	err = clGetPlatformIDs(1, &cl->platform, NULL);
	printf("Platform status: "); cl_error(err);
	if (err !=0) return;
	clGetPlatformInfo(cl->platform, CL_PLATFORM_NAME, 0, NULL, &size);
	clGetPlatformInfo(cl->platform, CL_PLATFORM_NAME, size, info, NULL);
	printf("Platform in use: %s\n", info);
	free(info);
}

void startDevice(CL *cl){
	cl_int err;
	size_t size;
	char *info = (char*) malloc (size*sizeof(char));
	
	err = clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_GPU, 1, &cl->device, NULL);
	if (err !=0){
		printf("GPU not found!\n");
		err = clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_CPU, 1, &cl->device, NULL);
	}
	printf("Device status: "); cl_error(err);
	clGetDeviceInfo(cl->device, CL_DEVICE_NAME, 0, NULL, &size);
	clGetDeviceInfo(cl->device, CL_DEVICE_NAME, size, info, NULL);
	printf("Device in use: %s\n", info);
	
	free(info);
}

void startContext(CL *cl){
	cl_int err;
	cl->context = clCreateContext(0, 1, &cl->device, NULL, NULL, &err);
	printf("Context status: "); cl_error(err);
}

void startCL(CL *cl){
	startPlatform(cl);
	startDevice(cl);
	startContext(cl);
}

void startQueue(CL *cl){
	cl_int err;
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &err);
	printf("Queue status: "); cl_error(err);
	printf("\n");
}

void releaseCL(CL *cl){
	clReleaseCommandQueue(cl->queue);
	clReleaseContext(cl->context);
}

void cl_invert(CL *cl, IplImage *img){
	cl_program program;
	cl_kernel kernel;
	cl_mem bufB;
	cl_mem bufC;
	cl_int err;
	int size = IMG_SIZE;
	char *hostC = (char*) malloc (size*sizeof(char));
	size_t globalSize[1] = {size};
	const char *krn = "./src/opencl/kernels/invert.cl";
	const char **source = getKernelPtr(krn);
	
	program = clCreateProgramWithSource(cl->context, 1, source, NULL, &err);
	printf("Program status: "); cl_error(err);
	clBuildProgram(program, 0, NULL, NULL, NULL, &err);
	printf("Build status: "); cl_error(err);
	kernel = clCreateKernel(program, "invert", &err);
	printf("Kernel status: "); cl_error(err);
	
	bufB = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, (size*sizeof(char)), NULL, &err);
	printf("bufB status: "); cl_error(err);
	bufC = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, (size*sizeof(char)), NULL, &err);
	printf("bufC status: "); cl_error(err);
	clEnqueueWriteBuffer(cl->queue, bufB, CL_TRUE, 0, (size*sizeof(char)), img->imageData, 0, NULL, NULL);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufC);
	clEnqueueNDRangeKernel(cl->queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	clFinish(cl->queue);
	clEnqueueReadBuffer(cl->queue, bufC, CL_TRUE, 0, size*sizeof(char), hostC, 0, NULL, NULL);
	printf("Enqueue buffer status: "); cl_error(err);
	for(int i=0; i<size; i++)
		img->imageData[i] = hostC[i];
	
	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
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
            printf("UNKNOWED CL ERROR CODE\n");
    }
}
