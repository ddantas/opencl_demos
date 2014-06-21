#include <stdio.h>
#include <stdlib.h>
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

cl_image_desc getDesc(CL* cl, IplImage* img){
	cl_int err;
	
	cl_mem imageData = (cl_mem)malloc(img->width*img->height);
	imageData = clCreateBuffer(cl->context, CL_MEM_COPY_HOST_PTR,
                        img->width*img->height,
                        img->imageData, &err);
    printf("BUFFER IMAGE STATUS: "); cl_error(err);
    cl_image_desc image_d;
    image_d.image_type = CL_MEM_OBJECT_IMAGE2D;
    image_d.image_width = (size_t)img->width;
    image_d.image_height = (size_t)img->height;
    image_d.image_depth = (size_t)img->depth;
    image_d.image_array_size = (size_t)(img->width*img->height);
    image_d.image_row_pitch = (size_t)img->width;
    image_d.image_slice_pitch = image_d.image_array_size;
    image_d.num_mip_levels = 0;
    image_d.num_samples = 0;
    image_d.buffer = imageData;
    
    return image_d;
}

void clInvert2D(CL* cl, IplImage* img){
	cl_int err;
    cl_image_desc desc = getDesc(cl, img);
    cl_image_desc descOut = getDesc(cl, img);
    
    cl_image_format src;
    cl_image_format out;
    src.image_channel_order = CL_A;
    src.image_channel_data_type = CL_UNORM_INT8;
    out.image_channel_order = CL_A;
    out.image_channel_data_type = CL_UNORM_INT8;
       
    cl_mem src_mem = 
    clCreateImage(cl->context, 0, &src, &desc, NULL, &err);
    printf("IMAGE STATUS SOURCE "); cl_error(err);
    
    cl_mem out_mem = 
	clCreateImage(cl->context, 0, &out, &descOut, NULL, &err);
	printf("IMAGE STATUS OUT "); cl_error(err);
    
    clGetMemObjectInfo(src_mem, CL_MEM_TYPE, sizeof(cl_int), &err, NULL);
    if(err == CL_MEM_OBJECT_IMAGE2D)
		printf("IMAGE TYPE: CL_MEM_OBJECT_IMAGE2D\n");
	
	size_t *src_origin=(size_t*)malloc(sizeof(size_t)*3);
	src_origin[0] = 0;
	src_origin[1] = 0;
	src_origin[2] = 0;
	
	size_t *src_region=(size_t*)malloc(sizeof(size_t)*3);
	src_region[0] = img->width;
	src_region[1] = img->height;
	src_region[2] = 1;
		
	err = clEnqueueWriteImage(cl->queue, src_mem, CL_TRUE,
	src_origin, src_region, sizeof(char)*img->width, 0, 
	(uint*)img->imageData,
	0, 0, NULL);
	printf("ENQUEUE IMAGE STATUS "); cl_error(err);
	
	cl_program program;
	cl_kernel kernel;
	
	const char* k = "./CLdemos/CL/Invert2.cl";
	const char** fonte = getKernelPtr(k);
	
	program = clCreateProgramWithSource(cl->context, 1, fonte, NULL, &err);
	printf("CREATE PROGRAM STATUS: "); cl_error(err);
	clBuildProgram(program, 0, NULL, NULL, NULL, &err);
	printf("BUILD PROGRAM STATUS: "); cl_error(err);
	kernel = clCreateKernel(program, "invert", &err);
	printf("KERNEL STATUS "); cl_error(err);
	
	
	err = clSetKernelArg( kernel, 0, sizeof( cl_mem ), (void *) &src_mem);
	printf("SET 1 KERNEL ARG "); cl_error(err);
	err = clSetKernelArg( kernel, 1, sizeof( cl_mem ), (void *) &out_mem);
	printf("SET 2 KERNEL ARG "); cl_error(err);

	size_t worksize[] = { img->width, img->height, 1};
	err = clEnqueueNDRangeKernel(cl->queue, kernel, 2, NULL, worksize,
	0, 0, 0, 0);
	printf("ENQUEUE ND KERNEL STATUS "); cl_error(err);
	
	clFinish(cl->queue);
	
	char* auxout = (char*)malloc(img->width*img->height*3);
	err = clEnqueueReadImage(cl->queue, out_mem, CL_TRUE, 
	src_origin, src_region, 0, 0, auxout, 0, NULL, NULL);
	printf("READ NEW IMAGE STATUS "); cl_error(err);
	
	for(int i=0; i<(img->width*img->height); i++)
        img->imageData[i] = auxout[i];
        
    free(auxout);    
    clReleaseKernel(kernel);
    clReleaseProgram(program);
}



void clInvertA(CL* cl, IplImage* img){
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
